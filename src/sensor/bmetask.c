#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "bmetask.h"
#include "abstractions/abstractions.h"
#include "freertos/semphr.h"
#include "json/json.h"
#include "bme280.h"
#include "stdio.h"

QueueHandle_t bme_queue;



void bme_read_task(void* pv) {

    bme_payload_t bme_data;
    for(;;) {
        uint8_t raw_data[8];
        uint8_t start_reg = 0xF7; 

       
        i2c_master_transmit_receive(bme_handle, &start_reg, 1, raw_data, 8, -1);

        int32_t adc_P = (raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4);
        int32_t adc_T = (raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4);
        int32_t adc_H = (raw_data[6] << 8)  | raw_data[7];


        bme_data.temp = calc_temp(adc_T, &cal); 
        bme_data.pres = calc_pressure(adc_P, &cal);
        bme_data.hum  = calc_humidity(adc_H, &cal);

        xQueueSend(bme_queue, &bme_data, 0); 

        vTaskDelay(pdMS_TO_TICKS(1000)); //1s read time thats why
    }
}