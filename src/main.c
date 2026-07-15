#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"


//other custom files
#include "mqtt/mqtt.h"
#include "wifi/wifi.h"
#include "abstractions/abstractions.h"
#include "json/json.h"


void sensor_send(void *pv) {
    float t=0, h=0, p = 0;

    for(;;) {
        
        send_payload(t,p,h);
        mutexPrint("PUBLISH", "Message sent!", 'I');
        vTaskDelay(pdMS_TO_TICKS(1000));
        t++, h++, p++;
    }
}

void app_main(void) {
    nvs_event_init(); //storing network config and attaching the callback functions to the events. default event loop too

    wifi_event_group = xEventGroupCreate();
    printMutex = xSemaphoreCreateMutex();

    init_wifi_hardware(); //hardware wifi conf
    wifi_conf(); //software wifi conf


    //gatekeeper
    ESP_LOGI("\nMAIN", "Waiting for WiFi...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("\nMAIN", "We are online!");

    mqtt_conf();

    xTaskCreatePinnedToCore(sensor_send, "jsonsend",2048, NULL, 1, NULL, 1);

    
}