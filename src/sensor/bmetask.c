#include "freertos/FreeRTOS.h"
#include "bmetask.h"
#include "abstractions/abstractions.h"
#include "freertos/semphr.h"

void bme_read_task(void* pv) {

    for(;;) {
        uint8_t raw_data[8];
        uint8_t start_reg = 0xF7; 

       
        i2c_master_transmit_receive(bme_handle, &start_reg, 1, raw_data, 8, -1);

        int32_t adc_P = (raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4);
        int32_t adc_T = (raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4);
        int32_t adc_H = (raw_data[6] << 8)  | raw_data[7];

        
        float temp = calc_temp(adc_T, &cal); 
        float pres = calc_pressure(adc_P, &cal);
        float hum  = calc_humidity(adc_H, &cal);

        if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
            ESP_LOGI("WEATHER", "Temp: %.2f C | Hum: %.1f %% | Pres: %.1f hPa", temp, hum, pres);
            xSemaphoreGive(printMutex);   
        }

        vTaskDelay(pdMS_TO_TICKS(5000)); //5s read time thats why
    }
}