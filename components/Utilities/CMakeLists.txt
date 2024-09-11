idf_component_register(
    SRCS
    
"nvsFlash/nvsFlash.c"
"nvsFlash/SpiffsManger.c"
"JsonUtility/JsonUtility.c"
"Log/Custom_Log.c"
"Buzzer/Buzzer.c"
"NetworkChecker/NetworkChecker.c"
 INCLUDE_DIRS 

"JsonUtility/include"
"Log"
"nvsFlash/include"
"Buzzer"
"NetworkChecker"
     REQUIRES spiffs nvs_flash esp_timer esp_event nvs_flash driver json esp_http_client esp_wifi) 