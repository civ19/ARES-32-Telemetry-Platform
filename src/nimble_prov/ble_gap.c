#include "ble_gap.h"
#include <string.h>
#include "esp_log.h"
#include "abstractions/abstractions.h"



static const char *TAG = "BLE_GAP";


void ble_app_advertise(void) {
    //clear buf
    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));

    //visibilitty and tax form
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    const char *dev_name = "ESP32_Prov_Node";
    fields.name = (uint8_t *)dev_name;
    fields.name_len = strlen(dev_name);
    fields.name_is_complete = 1;

    fields.uuids128 = BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, 
        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x11),
    fields.num_uuids128 = 1;
    fields.uuids128_is_complete = 1;

    int rc = ble_gap_adv_set_fields(&fields);
    if(rc != 0) {
        mutex_log("ADV", "Error setting advertisement fields. rc=%d", 'E', rc);
        return;
    }

    //execution of ad
    struct ble_gap_adv_params ad_params;
    memset(&ad_params, 0, sizeof(ad_params));
    ad_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    ad_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &ad_params, ble_gap_event, NULL);
    if(rc != 0) mutex_log("ADV", "Error setting advertisement. rc=%d", 'E', rc);
    else mutexPrint("ADV", "Ble Advertising started successfully. Waiitng for phone...", 'I');

}

static int ble_gap_event(struct ble_gap_event *event, void* arg) { //handle for gap events 

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if(event->connect.status == 0) mutexPrint("GAP", "Connection Established Successfully.", 'I');
            else {
                mutex_log("GAP", "Event connection failed. Error status: %d. Restarting advertisement...", 'E', event->connect.status);
                ble_app_advertise();
            }
            return 0;
        break;

        case BLE_GAP_EVENT_DISCONNECT:
            mutex_log("GAP", "Disconnected from client. Error status:%d", 'W', event->disconnect.reason);
            ble_app_advertise();
            return 0;
        
        break;

        case BLE_GAP_EVENT_MTU:
            mutex_log("GAP", "MTU size successfully negotiated %d bytes", event->mtu.value);
            return 0;

        break;

        default:
            return 0;
        break;


    }
    
   
}