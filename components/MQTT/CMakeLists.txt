idf_component_register(SRCS "MQTT_Interface.c" "MQTT_Core.c" "MQTT_Config.c"
                    INCLUDE_DIRS "include"
                    REQUIRES mqtt esp_event mbedtls nvs_flash json ) 
