#ifndef COFFEE_MAKER_APP_H_
#define COFFEE_MAKER_APP_H_
#include <stdint.h>
#include <sys/param.h>
#include "cJSON.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include <esp_event.h>
#include <esp_system.h>
#include "esp_log.h"

#include "MQTT_Interface.h"

#define COFFEE_TIME 120
#define COFFEE_MAKER_APP_SEC 1000

/**
 * @brief Structure to hold coffee maker application settings.
 * This structure contains various fields representing the settings and state of a coffee maker application,
 * including the grinder level, cup count, beverage flags, temperature, update time, and device credentials.
 */
typedef struct CoffeeMakerApp
{
    uint8_t GrinderLevel;
    uint8_t Cups;
    uint8_t TeaFlag;
    uint8_t CoffeeFlag;
    uint8_t Temp;
    uint16_t UpdateTime;
    char DeviceMACAddress[18];
    char Pass[18];
    char State[18];
} CoffeeMakerJson_str;

/**
 * @brief Main application function for managing the coffee maker's MQTT communication and user interface.
 * This function initializes the timer for the coffee maker application and handles MQTT subscriptions and messages.
 * It processes incoming data to update the coffee maker's state and display, as well as manages connection status.
 * @param MQTTDataFromBrokerQueue Pointer to the queue handle for receiving MQTT data from the *broker.
 * @param MQTTConnectedSemaphore Pointer to the semaphore handle indicating successful MQTT connection.
 * @param MQTTErrorOrDisconnectSemaphore Pointer to the semaphore handle indicating an MQTT error *or disconnection.
 * @return void
 */
void CoffeeMakerApplication(
    QueueHandle_t *MQTTDataFromBrokerQueue,
    SemaphoreHandle_t *MQTTConnectedSemaphore,
    SemaphoreHandle_t *MQTTErrorOrDisconnectSemaphore);

// #define COFFEE_MAKER_APP_TEST
#ifdef COFFEE_MAKER_APP_TEST
void PublishJsonForTest(char *CoffeeMakerJsonOutPut);
void JSON_TEST(CoffeeMakerJson_str *CoffeeMakerJson);
void parserTEST(char *temp);
#endif

#endif
