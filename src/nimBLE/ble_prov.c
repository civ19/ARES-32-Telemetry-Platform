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
    int rc = ble_hs_mbuf_to_flat(om, wifi_pass, len, NULL);
    if (rc!=0) return BLE_ATT_ERR_UNLIKELY; //if corrupted

    mutex_printf(TAG, "Successfully saved SSID", wifi_ssid);
    return 0;

}

static int pass_write(struct os_mbuf* om) { //om = network packets
    uint16_t len = OS_MBUF_PKTLEN(om);
    if(len > MAX_PASS_LEN) return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    
    memset(wifi_pass, 0, sizeof(wifi_pass)); //clearing the buffer
    
    int rc = ble_hs_mbuf_to_flat(om, wifi_pass, len, NULL);
    if(rc != 0) return BLE_ATT_ERR_UNLIKELY;
    mutex_printf(TAG, "Successfully saved password", wifi_pass);
    return 0;
}

static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctx, void* arg) {
    
    switch(ctx ->op) {
        case BLE_GATT_ACCESS_OP_WRITE_CHR:
            if(ble_uuid_cmp(ctx->chr->uuid, BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 
                0x49, 0x73, 0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x12))) return ssid_write;
            
            if(ble_uuid_cmp(ctx->chr->uuid, BLE_UUID128_DECLARE(0x2d, 0x71, 0xa1, 0x20, 0x53, 0x75, 0x49, 0x73, 
                        0xad, 0x57, 0x07, 0x72, 0xab, 0x39, 0x10, 0x13))) return pass_write;
        break;

        return BLE_ATT_ERR_ATTR_NOT_FOUND;

        case BLE_GATT_ACCESS_OP_READ_CHR:
            return BLE_ATT_ERR_READ_NOT_PERMITTED;
        break;

        default:
            return BLE_ATT_ERR_UNLIKELY;
    
    }
}

static int ble_gap_event(struct ble_gap_event *event, void* arg) {

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if(event->connect.status = 0) mutexPrint("GAP", "Connection Established Successfully.", 'I');
            else {
                mutex_printf("GAP", "Event connection failed. Error status: %d. Restarting advertisement...", event->connect.status);
                ble_app_advertise();
            }
            return 0;
        break;

        case BLE_GAP_EVENT_DISCONNECT:
            mutex_printf("GAP", "Disconnected from client. Error status:%d", event->disconnect.reason);
            ble_app_advertise();
            return 0;
        
        break;

        case BLE_GAP_EVENT_MTU:
            mutex_printf("GAP", "MTU size successfully negotiated %d bytes", event->mtu.value);
            return 0;

        break;
        
        default:
            return 0;
        break;


                
                

    }
    
   
}

