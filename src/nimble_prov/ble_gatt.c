#include "ble_gatt.h"
#include <string.h>
#include "esp_log.h"
#include "host/ble_hs.h"
#include "abstractions/abstractions.h"

static char wifi_ssid[MAX_SSID_LEN + 1]; //ssid and pass containers
static char wifi_pass[MAX_PASS_LEN + 1];

const char *TAG = "BLE_SVR";

static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, 
    struct ble_gatt_access_ctxt* ctx, void* arg);

void ble_app_advertise(void);
void wifi_conf(const char *dyn_ssid, const char *dyn_pass);


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