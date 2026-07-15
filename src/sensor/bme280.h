#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"


#pragma once

//the constants !
#define BME_ADDR            0x76   
#define BME_ID_REG          0xD0   
#define BME_CTRL_MEAS_REG   0xF4   
#define BME_TEMP_REG        0xFA   

typedef struct { //bsaically saying hey, this is the correction numbers basically to identify the exact bme 
    uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
    uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3; int16_t dig_P4;
    int16_t  dig_P5; int16_t dig_P6; int16_t dig_P7; int16_t dig_P8; int16_t dig_P9;
    uint8_t  dig_H1; int16_t dig_H2; uint8_t  dig_H3; int16_t dig_H4; int16_t dig_H5; int8_t dig_H6;
} bme280_calib_data;

extern int32_t t_fine; 
extern bme280_calib_data cal;
extern i2c_master_bus_handle_t bme_handle;

esp_err_t init_bme280();

float calc_temp(int32_t adc_T, bme280_calib_data *cal);
float calc_humidity(int32_t adc_H, bme280_calib_data *c);
float calc_pressure(int32_t adc_P, bme280_calib_data* c);
