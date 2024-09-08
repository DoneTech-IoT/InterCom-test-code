#ifndef DONE_COFFEE_MAKER_H
#define DONE_COFFEE_MAKER_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_matter.h>
#include "app_priv.h"

#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID 0x1001
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION 1

typedef enum
{
    INCREMENT_MODE = 0,
    DECREMENT_MODE = 1,
    EXPLICIT_MODE = 2
} LevelControlCurrentValueMode_t;

typedef enum
{
    POWER_KEY = 0,
    GRINDER_KEY = 1,
    COFFEE_KEY = 2,
    TEA_KEY = 3,
    CUP_COUNTER_KEY = 4,
    MICRO_SWITCH_KEY = 5
} Keys_t;

typedef enum
{
    DONT_CARE = 0,
    GRINDER_MODE = 1,
    COFFEE_MODE = 2,
    TEA_MODE = 3
} CookingMode_t;

typedef enum
{
    STANDBY_MODE = 1,
    ON_MODE = 2,
    PAUSE_MODE = 3
} PowerKeyMode_t;

typedef enum
{
    ERROR_MODE = 0,
    NORMAL_MODE = 1
} MicroSwitchMode_t;

typedef struct
{
    uint8_t Key;
    uint8_t State;
} KeyStatePair_t;

// Temp struct for Demo
typedef struct CoffeeMakerApp
{
    uint8_t GrinderLevel;
    uint8_t Cups;
    uint8_t TeaFlag;
    uint8_t CoffeeFlag;
    uint16_t UpdateTime;
} CoffeeMakerMatter_str;

typedef enum
{
    MICRO_SWITCH = 0,
    AUTO_TURN_OFF = 1,
    TIMER_COUNT
} TimerID_t;

/**
 * @brief This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 * @param[in] DriverHandle pointer to user data.
 * @param[in] EndpointID Endpoint ID of the attribute.
 * @param[in] ClusterID Cluster ID of the attribute.
 * @param[in] AttributeID Attribute ID of the attribute.
 * @param[in] Val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerAttributeUpdate(
    AppDriverHandle_t DriverHandle,
    const uint16_t &EndpointID, const uint32_t &ClusterID,
    const uint32_t &AttributeID, esp_matter_attr_val_t *Val);

/**
 * @brief create coffee maker device with its related endpoints
 * @param[in] Node Endpoint0 or root Node.
 * @param[in] QueueHandle queue for send event or data of device
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerCreate(
    esp_matter::node_t *Node,
    QueueHandle_t *QueueHandle);

#endif // DONE_COFFEE_MAKER_H