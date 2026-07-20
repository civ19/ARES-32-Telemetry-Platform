#pragma once

extern TaskHandle_t mqtt_uri_handle;
void mqtt_prov_task(void *pv);
void trigger_mqtt_prov(const char* dyn_mqtt_uri);