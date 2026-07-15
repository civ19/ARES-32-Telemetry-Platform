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
    pressure = (((pressure) - var2) * 3125) / var1;
    var1 = (((int64_t)c->dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (((int64_t)c->dig_P8) * pressure) >> 19;
    pressure = ((pressure) >> 8) + (((int64_t)c->dig_P7) << 4);
    return (float)pressure / 256.0f / 100.0f; //hpa
}

float calc_humidity(int32_t adc_H, bme280_calib_data *c) {
    int32_t pressure;
    pressure = (t_fine - ((int32_t)76800));
    pressure = (((((adc_H << 14) - (((int32_t)c->dig_H4) << 20) - (((int32_t)c->dig_H5) * pressure)) + 
                ((int32_t)16384)) >> 15) * (((((((pressure * ((int32_t)c->dig_H6)) >> 10) * 
                (((pressure * ((int32_t)c->dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + 
                ((int32_t)2097152)) * ((int32_t)c->dig_H2) + 8192) >> 14));
    pressure = (pressure - (((((pressure >> 15) * (pressure >> 15)) >> 7) * ((int32_t)c->dig_H1)) >> 4));
    pressure = (pressure < 0 ? 0 : pressure);
    pressure = (pressure > 419430400 ? 419430400 : pressure);
    return (float)(pressure >> 12) / 1024.0f;
}

esp_err_t init_bme280() {
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = (gpio_num_t)40, 
        .scl_io_num = (gpio_num_t)41, 
        .clk_source = I2C_CLK_SRC_DEFAULT, 
        .glitch_ignore_cnt = 7,
        .flags = { .enable_internal_pullup = 1 }
    };
    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &bus_handle);
    if (ret != ESP_OK) return ret;

    i2c_device_config_t dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BME_ADDR, 
        .scl_speed_hz = 400000, //its suppoosed to be fast
    };
    ret = i2c_master_bus_add_device(bus_handle, &dev_conf, &bme_handle);
    if (ret != ESP_OK) return ret;

    uint8_t start_cmd[2] = {BME_CTRL_MEAS_REG, 0x2E}; 
    ret = i2c_master_transmit(bme_handle, start_cmd, 2, -1);
    
    mutexPrint(TAGB, "BME280 Initialized and measuring.", 'I');
    return ret;
}




