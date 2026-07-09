#include "esp_event.h"
#pragma once

extern EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT (1 << 0)

void nvs_event_init();
void init_wifi_hardware();
void wifi_conf();