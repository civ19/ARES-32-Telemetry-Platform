#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"
#include "freertos/semphr.h"


//other custom files
#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "abstractions/abstractions.h"
#include "json/json.h"
#include "sensor/bme280.h"
#include "sensor/bmetask.h"
#include "nimble_prov/ble_master.h"




void app_main(void) {
    nvs_event_init(); //storing network config and attaching the callback functions to the events. default event loop too

    wifi_event_group = xEventGroupCreate();
    printMutex = xSemaphoreCreateMutex();
    bme_queue = xQueueCreate(10, sizeof(bme_payload_t));


    if(bme_queue == NULL || printMutex == NULL || wifi_event_group == NULL) {
        mutexPrint("MAIN", "Resource allocation failed.", 'E');
        return;
    }

    if(init_bme280() != ESP_OK) {
        mutexPrint("MAIN", "Failed to Init BME.", 'E');
        return;
    }

    init_wifi_hardware(); //hardware wifi conf

    if(init_ble_provisioning() != ESP_OK) {
        mutexPrint("MAIN", "Failed to init BLE provisioning stack", 'E');
        return;
    }


    //gatekeeper
    ESP_LOGI("\nMAIN", "BLE active. Waiting for phone BT provisioning...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("\nMAIN", "We are online!");

    mqtt_conf();

    xTaskCreatePinnedToCore(bme_read_task, "bmeReadTask",4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(bme_publish, "bmePublishTask", 4096, NULL, 1, NULL, 1);

    
}