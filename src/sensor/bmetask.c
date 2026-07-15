#include "freertos/FreeRTOS.h"


void bme_read_task(void* pv) {
    // PRE-LOOP: Read calibration data (simplified snippet)
    // You would use i2c_master_transmit_receive here to fill the 'cal' struct 
    // from registers 0x88 through 0xA1 and 0xE1 through 0xE7.

    while(1) {
        uint8_t raw_data[8];
        uint8_t start_reg = 0xF7; // Pressure starts here

        // 1. BURST READ 8 BYTES (Pres, Temp, Hum)
        i2c_master_transmit_receive(bme_handle, &start_reg, 1, raw_data, 8, -1);

        // 2. RECONSTRUCT RAW NUMBERS
        int32_t adc_P = (raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4);
        int32_t adc_T = (raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4);
        int32_t adc_H = (raw_data[6] << 8)  | raw_data[7];

        // 3. APPLY COMPENSATION
        float temp = compensate_temp(adc_T, &cal);
        float pres = compensate_pressure(adc_P, &cal);
        float hum  = compensate_humidity(adc_H, &cal);

        ESP_LOGI("WEATHER", "Temp: %.2f C | Hum: %.1f %% | Pres: %.1f hPa", 
                 temp, hum, pres);

        vTaskDelay(pdMS_TO_TICKS(5000)); // Read every 5 seconds
    }
}