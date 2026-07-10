#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "abstractions.h"
#include "esp_log.h"

SemaphoreHandle_t printMutex;

void mutexPrint(const char* TAG, const char* str, char type) {
    switch(type) {
       
        break;
        case 'I':
            if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                ESP_LOGI(TAG, "%s", str);
                xSemaphoreGive(printMutex);
            }
        break;
        case 'E':
            if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                ESP_LOGE(TAG, "%s", str);
                xSemaphoreGive(printMutex);
            }
        break;
    }
}