#include "esp_event.h"
#pragma once

#define WIFI_CONNECTED_BIT (1 << 0)


extern EventGroupHandle_t wifi_event_group;

void nvs_event_init();
void init_wifi_hardware();
void wifi_conf();


