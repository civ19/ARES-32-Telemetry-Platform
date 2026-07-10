#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"
#include "cJSON.h"

//other custom files
#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "abstractions/abstractions.h"




extern "C" void app_main(void) {
    nvs_event_init(); //storing network config and attaching the callback functions to the events. default event loop too

    wifi_event_group = xEventGroupCreate();
    printMutex = xSemaphoreCreateMutex();

    init_wifi_hardware(); //hardware wifi conf
    wifi_conf(); //software wifi conf


    //gatekeeper
    ESP_LOGI("MAIN", "Waiting for WiFi...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("MAIN", "We are online!");

    mqtt_conf();

    
}