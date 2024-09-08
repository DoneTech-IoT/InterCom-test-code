#ifdef __cplusplus
extern "C"
{
#endif
#ifndef MQTT_H_
#define MQTT_H_

#include "MQTT_Typedef.h"
#include "MQTT_Core.h"
#include "MQTT_Config.h"

#define MQTT_STACK 30 * 1000

#define SEC 1000

    /**
     * @brief Main task function for managing MQTT operations.
     * This task initializes semaphores and queues for MQTT operations, establishes
     * a thread connection, and handles various states such as initialization,
     * connection, publishing, subscribing, and disconnection.
     * @param pvParameter Parameters passed to the task (currently unused).
     * @return esp_err_t Status code indicating success or failure.
     */
    esp_err_t MQTT_Init(MQTT_Configuration_str *MQTT_InputConfigConfig);

    /**
     * @brief Subscribes to a specified MQTT topic.
     * Attempts to subscribe to the given topic if the MQTT connection is active.
     * @param Topic The topic to subscribe to.
     * @return true if MQTT network connected was successful, false otherwise.
     */
    bool MQTT_Subscribe(char *Topic);

    /**
     * @brief Unsubscribes from a specified MQTT topic.
     * Attempts to unsubscribe from the given topic if the MQTT connection is active.
     * @param Topic The topic to unsubscribe from.
     * @return true if MQTT network connected was successful, false otherwise.
     */
    bool MQTT_UnSubscribe(char *Topic);

    /**
     * @brief Publishes a message to a specified MQTT topic.
     * Publishes the given data to the specified topic if the MQTT connection is active.
     * @param Topic The topic to publish to.
     * @param Data The data to publish.
     * @return true if MQTT network connected was successful, false otherwise.
     */
    bool MQTT_Publish(char *Topic, char *Data);

    /**
     * @brief Disconnects from the MQTT broker.
     * This function handles the MQTT disconnection process.
     */
    void MQTT_Disconnect();

    /**
     * @brief Configures default MQTT settings.
     * This function initializes the MQTT interface with default configurations, including the queues and semaphores required for
     * handling incoming data from the broker, connection status, and error or disconnection events.
     *
     * @param MQTTDataFromBrokerQueue Queue handle for receiving data from the MQTT broker.
     * @param MQTTConnectedSemaphore Semaphore handle for signaling a successful MQTT connection.
     * @param MQTTErrorOrDisconnectSemaphore Semaphore handle for signaling an error or disconnection event.
     * @return true if the configuration was successful, false otherwise.
     */
    bool MQTT_DefaultConfig(
        QueueHandle_t *MQTTDataFromBrokerQueue,
        SemaphoreHandle_t *MQTTConnectedSemaphore,
        SemaphoreHandle_t *MQTTErrorOrDisconnectSemaphore);

// #define MQTT_TEST
#ifdef MQTT_TEST
    void TestPublishAndSubscribe();
    void test_MQtt();
#endif
#endif
#ifdef __cplusplus
}
#endif
