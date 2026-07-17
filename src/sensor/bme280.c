#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "bme280.h"

#include "abstractions/abstractions.h"

// Handles
i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t bme_handle;

int32_t t_fine; 
bme280_calib_data cal;

const char* TAGB = "BME_SENSOR";

void read_calibration_data() {
    uint8_t buf[24]; // Explicitly sized for your read length
    uint8_t reg = 0x88; 
    
    i2c_master_transmit_receive(bme_handle, &reg, 1, buf, 24, -1);

    cal.dig_T1 = (buf[1] << 8) | buf[0];
    cal.dig_T2 = (int16_t)((buf[3] << 8) | buf[2]);
    cal.dig_T3 = (int16_t)((buf[5] << 8) | buf[4]);

    cal.dig_P1 = (buf[7] << 8) | buf[6];
    cal.dig_P2 = (int16_t)((buf[9] << 8) | buf[8]);
    cal.dig_P3 = (int16_t)((buf[11] << 8) | buf[10]);
    cal.dig_P4 = (int16_t)((buf[13] << 8) | buf[12]);
    cal.dig_P5 = (int16_t)((buf[15] << 8) | buf[14]);
    cal.dig_P6 = (int16_t)((buf[17] << 8) | buf[16]);
    cal.dig_P7 = (int16_t)((buf[19] << 8) | buf[18]);
    cal.dig_P8 = (int16_t)((buf[21] << 8) | buf[20]);
    cal.dig_P9 = (int16_t)((buf[23] << 8) | buf[22]);

    uint8_t reg_h1 = 0xA1;
    i2c_master_transmit_receive(bme_handle, &reg_h1, 1, &cal.dig_H1, 1, -1);

    uint8_t reg_h2 = 0xE1;
    uint8_t h_buf[7];
    i2c_master_transmit_receive(bme_handle, &reg_h2, 1, h_buf, 7, -1);

    cal.dig_H2 = (int16_t)((h_buf[1] << 8) | h_buf[0]);
    cal.dig_H3 = h_buf[2];
    
    // bit masking for proper alignment of H4 and H5 12-bit values
    cal.dig_H4 = (int16_t)((int8_t)h_buf[3] << 4) | (h_buf[4] & 0x0F);
    cal.dig_H5 = (int16_t)((int8_t)h_buf[5] << 4) | ((h_buf[4] & 0xF0) >> 4);
    cal.dig_H6 = (int8_t)h_buf[6];
}



float calc_temp(int32_t adc_T, bme280_calib_data *cal) {
    int32_t var1, var2;

    var1 = ((((adc_T >> 3) - ((int32_t)cal->dig_T1 << 1))) * ((int32_t)cal->dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)cal->dig_T1)) * ((adc_T >> 4) - ((int32_t)cal->dig_T1))) >> 12) * ((int32_t)cal->dig_T3)) >> 14;
    
    t_fine = var1 + var2; 

    float temp = (t_fine * 5 + 128) >> 8;
    return temp / 100.0f; //celsius(as in, like, the real one, not the raw)
}

float calc_pressure(int32_t adc_P, bme280_calib_data* c) {
    int64_t var1, var2, pressure;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)c->dig_P6;
    var2 = var2+((var1 * (int64_t)c->dig_P5) << 17);
    var2 = var2 + (((int64_t)c->dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)c->dig_P3) >> 8) + ((var1 * (int64_t)c->dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)c->dig_P1) >> 33;
    if (var1 == 0) return 0;
    pressure = 1048576 - adc_P;
    pressure = (((((int64_t)pressure) << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)c->dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (((int64_t)c->dig_P8) * pressure) >> 19;
    pressure = ((pressure) >> 8) + (((int64_t)c->dig_P7) << 4);
    return (float)pressure / 256.0f / 100.0f; //hpa
}

float calc_humidity(int32_t adc_H, bme280_calib_data *c) {
    // 1. Convert to double for precision - avoids all integer overflow issues
    double var_H;
    
    // The Bosch formula translated for a modern CPU
    var_H = (((double)t_fine) - 76800.0);
    var_H = (adc_H - (((double)c->dig_H4) * 64.0 + ((double)c->dig_H5) / 16384.0 * var_H)) *
            (((double)c->dig_H2) / 65536.0 * (1.0 + ((double)c->dig_H6) / 67108864.0 * var_H *
            (1.0 + ((double)c->dig_H3) / 67108864.0 * var_H)));
            
    var_H = var_H * (1.0 - ((double)c->dig_H1) * var_H / 524288.0);
    
    // 2. Bound the result to physical reality (0% to 100%)
    if (var_H > 100.0) var_H = 100.0;
    else if (var_H < 0.0) var_H = 0.0;
    
    return (float)var_H;
}

esp_err_t init_bme280() {
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = (gpio_num_t)41, 
        .scl_io_num = (gpio_num_t)42, 
        .clk_source = I2C_CLK_SRC_DEFAULT, 
        .glitch_ignore_cnt = 7,
        .flags = { .enable_internal_pullup = 1 }
    };
    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &bus_handle);
    if (ret != ESP_OK) return ret;

    i2c_device_config_t dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BME_ADDR, 
        .scl_speed_hz = 400000, 
    };
    ret = i2c_master_bus_add_device(bus_handle, &dev_conf, &bme_handle);
    if (ret != ESP_OK) return ret;

    read_calibration_data(); 

    // 1. MUST write humidity control register first
    uint8_t hum_cmd[2] = {0xF2, 0x01}; // Humidity oversampling x1
    i2c_master_transmit(bme_handle, hum_cmd, 2, -1);
    
    vTaskDelay(pdMS_TO_TICKS(10)); // Tiny structural delay for safety

    // 2. Transmit to ctrl_meas with 0x2F to commit previous register changes AND set normal mode
    uint8_t start_cmd[2] = {BME_CTRL_MEAS_REG, 0x2F}; 
    ret = i2c_master_transmit(bme_handle, start_cmd, 2, -1);
    
    mutexPrint(TAGB, "BME280 Initialized and measuring continuously.", 'I');
    return ret;
}




