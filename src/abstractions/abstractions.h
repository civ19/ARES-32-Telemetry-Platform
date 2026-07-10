#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SemaphoreHandle_t printMutex;

void mutexPrint(const char* TAG, const char* str, char type);

#ifdef __cplusplus
}
#endif