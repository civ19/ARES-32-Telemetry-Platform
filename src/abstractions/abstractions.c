#include "abstractions.h" // Ensure this matches your header filename exactly
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"

// Explicit linkage block for global C symbols

SemaphoreHandle_t printMutex = NULL;

void mutexPrint(const char* TAG, const char* str, char type) {
    switch(type) {
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
        default:
            break;
    }
}

