idf_component_register(
    SRCS
        "main.c"
        "mqtt/mqtt.c"
        "wifi/wifi.c"
        "abstractions/abstractions.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        driver
        esp_adc
        mqtt
        cjson
)