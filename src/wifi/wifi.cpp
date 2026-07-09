#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi.h" // Defines WIFI_EVENT
#include "esp_netif.h" // Defines IP_EVENT
#include "freertos/event_groups.h"

#include "abstractions.h"
#include "wifi.h"

static const char *TAG = "EVENT_LOOP";
EventGroupHandle_t wifi_event_group = NULL;
SemaphoreHandle_t printMutex = NULL;


static uint8_t retry_ctr = 0;
static uint8_t max_retry = 5;
void reconnect() {
    if(retry_ctr < max_retry) {
        esp_wifi_connect();
        retry_ctr++;
        
        if(xSemaphoreTake(printMutex, portMAX_DELAY)){
            ESP_LOGI(TAG, "Retry to connect to AP: (%d/%d)", retry_ctr, max_retry);
            xSemaphoreGive(printMutex);
        }
    }
    else {
        if(xSemaphoreTake(printMutex, portMAX_DELAY)){
            ESP_LOGE(TAG, "Failed to connect after maximum retries.");
            xSemaphoreGive(printMutex);
        }
    }
}

static void wifi_event_callback(void *arg, esp_event_base_t dept, int32_t event_id, void* data) { //manages wifi state
    if(dept == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
            ESP_LOGW(TAG, "Disconnected. Reconnecting...");
            xSemaphoreGive(printMutex);
            
        }
        reconnect(); //reconnect asap
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT); //not ready. clearing bit 0
       
    }

    else if(dept == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        retry_ctr = 0;
        ip_event_got_ip_t *event_data = (ip_event_got_ip_t* )data;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        if(xSemaphoreTake(printMutex, portMAX_DELAY)) { 
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event_data->ip_info.ip));
            xSemaphoreGive(printMutex);
        }
    }

}

void nvs_event_init() {
    //storing network config here, and sets the 'interrupts'/event drivers for the events
    esp_err_t ret = nvs_flash_init();
    //ersae if the conditions apply
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret); //since ret returns esp_err_t. we dont want it to keep running if it fails

    //default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default()); //default loop 

    //if anything happens in wifi event dept, call this function. kinda like attaching a function to an interrupt
    
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_callback, NULL, NULL));

    //registerfor ip events too
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifi_event_callback, NULL, NULL));


}

void init_wifi_hardware() {
    //make the tcp/ip stack
    ESP_ERROR_CHECK(esp_netif_init());
    //create the station => esp becoming a client for the wifi
    esp_netif_create_default_wifi_sta();

    //wifi conf init
    wifi_init_config_t wifi_conf = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_conf));

    //wifi set mode. basically confirming its a station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); 

}

void wifi_conf() {
    wifi_config_t wifi_conf = {};
    strlcpy((char*)wifi_conf.sta.ssid, "1051A", sizeof(wifi_conf.sta.ssid));
    strlcpy((char*)wifi_conf.sta.password, "Secord1051A", sizeof(wifi_conf.sta.password));

    //set the config to the hardware
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_conf));
    //power on radio
    ESP_ERROR_CHECK(esp_wifi_start());
    //connect to the actual router
    ESP_ERROR_CHECK(esp_wifi_connect());

}


