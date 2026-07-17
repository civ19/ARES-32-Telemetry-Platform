#include "host/ble_hs.h"
#include "services/gatt/ble_svc_gatt.h"
#include <string.h>

#include "abstractions/abstractions.h"
#include "esp_log.h"
#include "ble_prov.h"

static char wifi_ssid[MAX_SSID_LEN + 1]; //ssid and pass containers
static char wifi_pass[MAX_PASS_LEN + 1];

const char *TAG = "BLE_SVR";

static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, 
    struct ble_gatt_access_ctxt* ctx, void* arg);

void ble_app_advertise(void);


static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, //prov service
                        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x11),
        
        .characteristics = prov_features,
    },
    {.type = BLE_GATT_SVC_TYPE_END}
    
};

static const struct ble_gatt_chr_def prov_features[] = {
    { //ssid
        .uuid = BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, 
                        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x12),
        .access_cb = gatt_svr_access_cb,
            .flags = BLE_GATT_CHR_F_WRITE,
    },


    { //pass
        .uuid = BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, 
                        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x13),
        .access_cb = gatt_svr_access_cb,
        .flags = BLE_GATT_CHR_F_WRITE,
    },
    {.uuid = NULL} //signla to stop processing

};

static int ssid_write(struct os_mbuf *om) {
    uint16_t len = OS_MBUF_PKTLEN(om); //length of the incoming packet via ble
    if(len > MAX_SSID_LEN) return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;

    memset(wifi_ssid, 0, sizeof(wifi_ssid)); //clean slate
    int rc = ble_hs_mbuf_to_flat(om, wifi_ssid, len, NULL);
    if (rc!=0) return BLE_ATT_ERR_UNLIKELY; //if corrupted

    mutex_log(TAG, "Successfully saved SSID", wifi_ssid);
    return 0;

}

static int pass_write(struct os_mbuf* om) { //om = network packets
    uint16_t len = OS_MBUF_PKTLEN(om);
    if(len > MAX_PASS_LEN) return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    
    memset(wifi_pass, 0, sizeof(wifi_pass)); //clearing the buffer
    
    int rc = ble_hs_mbuf_to_flat(om, wifi_pass, len, NULL);
    if(rc != 0) return BLE_ATT_ERR_UNLIKELY;
    mutex_log(TAG, "Successfully saved password", wifi_pass);
    return 0;
}

static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctx, void* arg) {
    
    switch(ctx ->op) {
        case BLE_GATT_ACCESS_OP_WRITE_CHR:
            if(ble_uuid_cmp(ctx->chr->uuid, BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 
                0x49, 0x73, 0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x12))) return ssid_write(ctx->om);
            
            if(ble_uuid_cmp(ctx->chr->uuid, BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, 
                        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x13))) return pass_write(ctx->om);
        break;

        return BLE_ATT_ERR_ATTR_NOT_FOUND;

        case BLE_GATT_ACCESS_OP_READ_CHR:
            return BLE_ATT_ERR_READ_NOT_PERMITTED;
        break;

        default:
            return BLE_ATT_ERR_UNLIKELY;
    
    }
}

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

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &ad_params, gatt_svr_access_cb, NULL);
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

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define TAGB "BLE_INIT"

static uint8_t own_addr_type = BLE_OWN_ADDR_PUBLIC;

//sync cb
static void ble_app_on_sync(void) {
    int rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if(rc != 0) {
        mutex_log(TAGB, "Fatal: Failed to infer ble address type. rc=%d", 'E', rc);
        return;
    }

    mutexPrint(TAGB, "Hardware sync complete. Addr type verified.", 'I');
    ble_app_advertise();

}

//host task
void ble_host_task(void *param) {
    mutex_log(TAGB, "BLE host worker loop launched on Core %d", 'I', xPortGetCoreID());
    nimble_port_run();

    mutexPrint(TAGB, "NimBLE event loop exited. Destroyed thread context...", 'W');
    vTaskDelete(NULL);
}


esp_err_t init_ble_provisioning(void) {

    int rc = nimble_port_init();
    if(rc!=0) {
        mutex_log(TAGB, "Init failed! Memory alloc arror: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    ble_hs_cfg.sync_cb = ble_app_on_sync; //attaching synbc cb 

    ble_svc_gap_init();
    ble_svc_gatt_init();

    //profile structure init and validation
    rc = ble_gatts_count_cfg(gatt_svr_svcs); //allocating the right memory for tghe gatt table
    if(rc!=0) {
        mutex_log(TAGB, "GATT Table Memory alloc arror: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if(rc!=0) {
        mutex_log(TAGB, "Failed to add GATT table to BT Database pool: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    //name for dev
    rc = ble_svc_gap_device_name_set("S3-WEATHER-STATION");
    if(rc!=0) {
        mutex_log(TAGB, "Failed to apply public gap node name: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    //task on core 0
    mutexPrint(TAGB, "Starting async BT task...", 'I');
    nimble_port_freertos_init(ble_host_task);

    return ESP_OK;


}




