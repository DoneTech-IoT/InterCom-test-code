#ifdef __cplusplus
extern "C" {
#endif

#ifndef MQTT_TYPEDEF_H_
#define MQTT_TYPEDEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

#include "esp_freertos_hooks.h"
#include "esp_system.h"
#include "esp_log.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "mqtt_client.h"

#define MQTT_SEC 1000
// Define constants for topic length and raw data length
#define TOPIC_LEN 50
#define MQTT_RAW_DATA_LEN 2500

// Define the size of the user property array
#define USE_PROPERTY_ARR_SIZE 1

// Define states for MQTT operation
#define MQTT_INIT                0
#define MQTT_CONNECTED           1
#define MQTT_PUBLISH             2
#define MQTT_SUBSCRIBE           3
#define MQTT_DATA_CONNECTION     4
#define MQTT_DISCONNECT          5
#define MQTT_UNSUBSCRIBE         6
#define MQTT_KILL                7
#define MQTT_ERROR               8

/**
 * @brief Structure to hold MQTT data, including topic and raw data.
 */
typedef struct
{
    char Topic[TOPIC_LEN];
    char RawData[MQTT_RAW_DATA_LEN];
} MQTT_Data_str;

/**
 * @brief Configuration structure for MQTT subscriptions.
 */
typedef struct
{
    char Topic[TOPIC_LEN];
    int Qos;    // quality of service
    int Retain; // store packet

} SubscriptionConfig_str;

/**
 * @brief Configuration structure for MQTT publishing.
 */ 
typedef struct
{
    char Topic[TOPIC_LEN];
    int Qos;    // quality of service
    int Retain; // store packet
} PublishConfig_str;

/**
 * @brief Main configuration structure for MQTT operations.
 */
typedef struct
{
    SubscriptionConfig_str *SubscriptionConfig;
    PublishConfig_str *PublishConfig;
    uint16_t ClientID;
    QueueHandle_t *DataFromBrokerQueue;
    SemaphoreHandle_t *ConnectedSemaphore;
    SemaphoreHandle_t *ErrorOrDisconnectSemaphore;
} MQTT_Configuration_str;

/**
 * @brief Structure to manage MQTT interface, including configuration and properties.
 */
typedef struct
{
    MQTT_Configuration_str MQTT_Config;
    esp_mqtt5_publish_property_config_t PublishProperty;
    esp_mqtt5_subscribe_property_config_t SubscribeProperty;
    esp_mqtt5_unsubscribe_property_config_t UnsubscribeProperty;
    esp_mqtt5_disconnect_property_config_t DisconnectProperty;
    esp_mqtt_client_handle_t MainClient;
    uint8_t ConfigIsTrueFlag ;
    uint8_t IsConnectedFlag ;
} MQTT_Interface_str;


/**
 * @brief Array of user properties for MQTT version 5.
 */
extern esp_mqtt5_user_property_item_t UserPropertyArray[USE_PROPERTY_ARR_SIZE];

/**
 * @brief Structure to manage MQTT thread connection.
 */
typedef struct 
{
    SemaphoreHandle_t MQTT_ConnectedSemaphore ;
    SemaphoreHandle_t MQTT_DisconnectedSemaphore;
    SemaphoreHandle_t MQTT_BrokerDataSemaphore;
    SemaphoreHandle_t MQTT_ErrorInCoreSemaphore;
    QueueHandle_t MQTT_BufQueue;
}MQTT_ThreadConnection_str;





#endif
#ifdef __cplusplus
}
#endif