idf_component_register(
    SRCS
        "main.c"
        "mqtt/mqtt.c"
        "wifi/wifi.c"
        "abstractions/abstractions.c"
        "json/json.c"
        "sensor/bme280.c"
        "sensor/bmetask.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        driver
        esp_adc
        mqtt
        cjson
)