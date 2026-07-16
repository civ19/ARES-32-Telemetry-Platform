#include "host/ble_hs.h"
#include "services/gatt/ble_svc_gatt.h"


static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, 
    struct ble_gatt_access_ctxt *ctxt, void* arg);


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

