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

float temp_calculate(int32_t adc_T, bme280_calib_data *cal) {
    int32_t var1, var2;

    // THE MATH YOU RECOGNIZED:
    var1 = ((((adc_T >> 3) - ((int32_t)cal->dig_T1 << 1))) * ((int32_t)cal->dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)cal->dig_T1)) * ((adc_T >> 4) - ((int32_t)cal->dig_T1))) >> 12) * ((int32_t)cal->dig_T3)) >> 14;
    
    
    t_fine = var1 + var2; 

    float T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f; // Return Celsius
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
        .scl_speed_hz = 400000, // Fast Mode
    };
    ret = i2c_master_bus_add_device(bus_handle, &dev_conf, &bme_handle);
    if (ret != ESP_OK) return ret;

    uint8_t start_cmd[2] = {BME_CTRL_MEAS_REG, 0x2E}; 
    ret = i2c_master_transmit(bme_handle, start_cmd, 2, -1);
    
    mutexPrint(TAGB, "BME280 Initialized and measuring.", 'I');
    return ret;
}




