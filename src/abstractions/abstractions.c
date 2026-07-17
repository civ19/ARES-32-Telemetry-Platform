#include "abstractions.h" // Ensure this matches your header filename exactly
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"


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


void mutex_log(char type, const char *tag, const char *format, ...) {
    esp_log_level_t level;
    switch (type) {
        case 'E': level = ESP_LOG_ERROR;  break; 
        case 'W': level = ESP_LOG_WARN;   break;
        case 'I': level = ESP_LOG_INFO;   break; 
        case 'D': level = ESP_LOG_DEBUG;  break; 
        default:  level = ESP_LOG_INFO;   break;
    }

    if (printMutex == NULL) {
        va_list args;
        va_start(args, format); 
        esp_log_writev(level, tag, format, args);
        va_end(args);
        return;
    }

    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
        va_list args;
        va_start(args, format); 

        esp_log_writev(level, tag, format, args);

        va_end(args);
        xSemaphoreGive(printMutex);
    }
}