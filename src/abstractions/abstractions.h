#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t printMutex;

void mutexPrint(const char* TAG, const char* str, char type);
void mutex_log(const char *tag, const char *format, char type, ...);