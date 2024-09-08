#include <stdio.h>
#include "MQTT_Interface.h"

// Tag used for logging MQTT interface messages
static const char *TAG = "MQTT_Interface";

// Static buffer and stack for the MQTT task
StaticTask_t *xTaskMQTTBuffer;
StackType_t *xMQTTStack;

// Structure holding MQTT interface configurations and state
MQTT_Interface_str MQTT_Interface;
// Array of user properties for MQTT messages
esp_mqtt5_user_property_item_t UserPropertyArray[USE_PROPERTY_ARR_SIZE];
// Structure holding MQTT thread connection data
MQTT_ThreadConnection_str MQTT_ThreadConnection;

/**
 * @brief Internal function to establish MQTT thread connection.
 * Initializes semaphores and queue for MQTT thread communication.
 */
static void MQTT_InterfaceAndCoreConnection()
{
    MQTT_ThreadConnection.MQTT_ConnectedSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_DisconnectedSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_BrokerDataSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_ErrorInCoreSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_BufQueue = xQueueCreate(1, sizeof(MQTT_Data_str));
    esp_err_t error = MQTTCore_ThreadConnectionMaker(&MQTT_ThreadConnection);
    if (error == ESP_FAIL)
        ESP_LOGE(TAG, "can not make thread connection");
}

/**
 * @brief Subscribes to a specified MQTT topic.
 * Attempts to subscribe to the given topic if the MQTT connection is active.
 * @param Topic The topic to subscribe to.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Subscribe(char *Topic)
{
    if (MQTT_Interface.IsConnectedFlag == false)
    {
        ESP_LOGE(TAG, "MQTT is not connected");
        return false;
    }
    char topic[TOPIC_LEN] = {0};
    strcpy(topic, Topic);
    MQTTCore_Subscriber(topic,
                        &MQTT_Interface.MainClient,
                        &MQTT_Interface.SubscribeProperty,
                        UserPropertyArray,
                        &MQTT_Interface.SubscribeProperty.user_property);
    return true;
}

/**
 * @brief Unsubscribes from a specified MQTT topic.
 * Attempts to unsubscribe from the given topic if the MQTT connection is active.
 * @param Topic The topic to unsubscribe from.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_UnSubscribe(char *Topic)
{
    if (MQTT_Interface.IsConnectedFlag == false)
    {
        ESP_LOGE(TAG, "MQTT is not connected");
        return false;
    }
    char topic[TOPIC_LEN] = {0};
    strcpy(topic, Topic);
    MQTTCore_Unsubscribe(topic,
                         &MQTT_Interface.MainClient,
                         UserPropertyArray,
                         &MQTT_Interface.UnsubscribeProperty,
                         &MQTT_Interface.UnsubscribeProperty.user_property);
    return true;
}

/**
 * @brief Publishes a message to a specified MQTT topic.
 * Publishes the given data to the specified topic if the MQTT connection is active.
 * @param Topic The topic to publish to.
 * @param Data The data to publish.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Publish(char *Topic, char *Data)
{
    if (MQTT_Interface.IsConnectedFlag == false)
    {
        ESP_LOGE(TAG, "MQTT is not connected");
        return false;
    }
    MQTT_Data_str MQTT_Data;
    memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
    memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
    strcpy(MQTT_Data.Topic, Topic);
    strcpy(MQTT_Data.RawData, Data);
    MQTTCore_Publisher(MQTT_Data.Topic,
                       MQTT_Data.RawData,
                       &MQTT_Interface.MainClient,
                       UserPropertyArray,
                       &MQTT_Interface.PublishProperty,
                       &MQTT_Interface.PublishProperty.user_property,
                       MQTT_Interface.MQTT_Config.PublishConfig->Qos,
                       MQTT_Interface.MQTT_Config.PublishConfig->Retain);
    return true;
}

/**
 * @brief Disconnects from the MQTT broker.
 * This function handles the MQTT disconnection process.
 */
void MQTT_Disconnect()
{
    if (MQTT_Interface.IsConnectedFlag == false)
    {
        ESP_LOGE(TAG, "MQTT is disconnect !");
        return;
    }
    MQTTCore_Disconnect(&MQTT_Interface.MainClient,
                        UserPropertyArray,
                        &MQTT_Interface.DisconnectProperty,
                        &MQTT_Interface.DisconnectProperty.user_property);
}

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
    SemaphoreHandle_t *MQTTErrorOrDisconnectSemaphore)
{
    static MQTT_Configuration_str Interface;
    static SubscriptionConfig_str SubscriptionConfig;
    static PublishConfig_str PublishConfig;
    SubscriptionConfig.Qos = 1;
    SubscriptionConfig.Retain = 0;
    strcpy(SubscriptionConfig.Topic, "AndroidApp/TV");

    PublishConfig.Qos = 1;
    PublishConfig.Retain = 0;
    strcpy(PublishConfig.Topic, "Device");

    Interface.ClientID = 2225;
    Interface.PublishConfig = &PublishConfig;
    Interface.SubscriptionConfig = &SubscriptionConfig;

    Interface.ConnectedSemaphore = MQTTConnectedSemaphore;
    Interface.DataFromBrokerQueue = MQTTDataFromBrokerQueue;
    Interface.ErrorOrDisconnectSemaphore = MQTTErrorOrDisconnectSemaphore;
    esp_err_t error;
    error = MQTT_Init(&Interface);
    if (error == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT Inited!");
        return true;
    }
    ESP_LOGE(TAG, "MQTT iit fail!");
    return false;
}

/**
 * @brief Main task function for managing MQTT operations.
 * This task initializes semaphores and queues for MQTT operations, establishes
 * a thread connection, and handles various states such as initialization,
 * connection, publishing, subscribing, and disconnection.
 * @param pvParameter Parameters passed to the task (currently unused).
 * @return void
 */
void MQTT_InterfaceTask(void *pvParameter)
{
    ESP_LOGI(TAG, "MQTT task gets running");
    TickType_t WaitForSemaphore = 1;
    int Counter = 0;

#ifdef MQTT_TEST
    uint8_t testFlag = false;
#endif
    MQTT_InterfaceAndCoreConnection();
    MQTT_ConfigFunction(UserPropertyArray, &MQTT_Interface);
    int State = MQTT_INIT;
    while (true)
    {
        switch (State)
        {
        case MQTT_INIT:
        {
            ESP_LOGI(TAG, "state MQTT_INIT");
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_ConnectedSemaphore, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
            {
                xSemaphoreGive((*MQTT_Interface.MQTT_Config.ConnectedSemaphore));
                MQTT_Interface.IsConnectedFlag = true;
                State = MQTT_CONNECTED;
                ESP_LOGI(TAG, "state MQTT_CONNECTED");
                break;
            }
            Counter++;
            ESP_LOGE(TAG, "try to connect MQTT network= %d", Counter);
            if (Counter == TRY_TO_CONNECT)
            {
                State = MQTT_DISCONNECT;
                break;
            }
            State = MQTT_INIT;
            break;
        }
        case MQTT_CONNECTED:
        {
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_ErrorInCoreSemaphore, WaitForSemaphore) == pdTRUE)
            {
                State = MQTT_ERROR;
            }
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_BrokerDataSemaphore, WaitForSemaphore) == pdTRUE)
            {
                ESP_LOGI(TAG, "state MQTT_BrokerDataSemaphore");
                State = MQTT_DATA_CONNECTION;
            }
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_DisconnectedSemaphore, WaitForSemaphore) == pdTRUE)
            {
                State = MQTT_DISCONNECT;
            }
#ifdef MQTT_TEST
            if (testFlag == false)
            {
                TestPublishAndSubscribe();
                vTaskDelay(pdMS_TO_TICKS(30 * 1000));
                testFlag = true;
            }
            // TestPublishAndSubscribe();
#endif

            break;
        }
        case MQTT_DATA_CONNECTION:
        {

            ESP_LOGI(TAG, "state MQTT_DATA_CONNECTION");
            MQTT_Data_str MQTT_Data;
            memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
            memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
            BaseType_t error = xQueueReceive(MQTT_ThreadConnection.MQTT_BufQueue, &MQTT_Data, pdMS_TO_TICKS(MQTT_SEC));
            if (error != pdTRUE)
            {
                ESP_LOGE(TAG, "Receive data from broker failed");
                State = MQTT_CONNECTED;
                break;
            }
            error = xQueueSend(*(MQTT_Interface.MQTT_Config.DataFromBrokerQueue), MQTT_Data.RawData, pdMS_TO_TICKS(MQTT_SEC));
            if (error != pdTRUE)
            {
                ESP_LOGE(TAG, "Send data to app layer failed");
                State = MQTT_CONNECTED;
                break;
            }
            ESP_LOGI(TAG, "Sended data to the app layer ");
            State = MQTT_CONNECTED;
            break;
        }
        case MQTT_DISCONNECT:
        {
            ESP_LOGE(TAG, "state MQTT_DISCONNECT");
            xSemaphoreGive(*MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore);
            State = MQTT_KILL;
            break;
        }
        case MQTT_KILL:
        {
            ESP_LOGE(TAG, "state MQTT_KILL");
            vTaskSuspend(NULL);
            break;
        }
        case MQTT_ERROR:
        {
            ESP_LOGI(TAG, "state MQTT_ERROR");
            xSemaphoreGive(*MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore);
            State = MQTT_CONNECTED;
            break;
        }
        }
    }
}

/**
 * @brief Initializes and creates the MQTT task.
 * This function sets up the MQTT task with the specified priority and stack size,
 * if the configuration flag is set. It allocates memory for the task stack and control block
 * and creates the task statically.
 * @param MQTT_TaskHandler Pointer to the task handle for the created task.
 * @param TaskPriority Priority of the task.
 * @param TaskStack Stack size for the task.
 * @return void
 */
void MQTT_TaskInit(TaskHandle_t *MQTT_TaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack)
{
    if (MQTT_Interface.ConfigIsTrueFlag == false)
    {
        ESP_LOGE(TAG, "MQTT task need config !");
        return;
    }
    xTaskMQTTBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xMQTTStack = (StackType_t *)malloc(TaskStack * sizeof(StackType_t));
    *MQTT_TaskHandler = xTaskCreateStatic(
        MQTT_InterfaceTask,   // Task function
        "MQTT_InterfaceTask", // Task name (for debugging)
        TaskStack,            // Stack size (in words)
        NULL,                 // Task parameters (passed to the task function)
        TaskPriority,         // Task priority (adjust as needed)
        xMQTTStack,           // Stack buffer
        xTaskMQTTBuffer       // Task control block
    );
    ESP_LOGI(TAG, "MQTT task successfully created!");
}

/**
 * @brief Initializes the MQTT interface with the provided configuration.
 * This function sets up the MQTT interface using the specified configuration structure.
 * It validates the configuration and, if valid, initializes the task and sets the configuration flag.
 * @param MQTT_InputConfig Pointer to the configuration structure for MQTT.
 * @return esp_err_t Status code indicating success or failure.
 */
esp_err_t MQTT_Init(MQTT_Configuration_str *MQTT_InputConfig)
{
    if (MQTT_InputConfig->ConnectedSemaphore != NULL &&
        MQTT_InputConfig->DataFromBrokerQueue != NULL &&
        MQTT_InputConfig->ErrorOrDisconnectSemaphore != NULL &&
        MQTT_InputConfig->PublishConfig != NULL &&
        MQTT_InputConfig->SubscriptionConfig != NULL)
    {

        MQTT_Interface.MQTT_Config.ClientID = MQTT_InputConfig->ClientID;
        MQTT_Interface.MQTT_Config.ConnectedSemaphore = MQTT_InputConfig->ConnectedSemaphore;
        MQTT_Interface.MQTT_Config.DataFromBrokerQueue = MQTT_InputConfig->DataFromBrokerQueue;
        MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore = MQTT_InputConfig->ErrorOrDisconnectSemaphore;
        MQTT_Interface.MQTT_Config.PublishConfig = MQTT_InputConfig->PublishConfig;
        MQTT_Interface.MQTT_Config.SubscriptionConfig = MQTT_InputConfig->SubscriptionConfig;

        *(MQTT_Interface.MQTT_Config.ConnectedSemaphore) = xSemaphoreCreateBinary();
        *(MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore) = xSemaphoreCreateBinary();
        *(MQTT_Interface.MQTT_Config.DataFromBrokerQueue) = xQueueCreate(1, MQTT_RAW_DATA_LEN * sizeof(char));
        MQTT_Interface.IsConnectedFlag = false;
        MQTT_Interface.ConfigIsTrueFlag = true;
        /*in future we check all config */
        /* To do ** save in spiffs */
        TaskHandle_t MQTT_TaskHandler;
        UBaseType_t TaskPriority = tskIDLE_PRIORITY + 1;
        uint32_t TaskStack = MQTT_STACK;
        /* At future this task should be create by service manager*/
        MQTT_TaskInit(&MQTT_TaskHandler, TaskPriority, MQTT_STACK);

        return ESP_OK;
    }
    else
        return ESP_FAIL;
}

/**
 * @brief Deletes the MQTT task and frees associated resources.
 * This function deletes the specified MQTT task and releases the memory allocated for
 * the task stack and control block.
 * @param TaskHandler Pointer to the task handle to be deleted.
 * @return void
 */
void MQTT_TaskKill(TaskHandle_t *TaskHandler)
{
    if (TaskHandler == NULL)
    {
        ESP_LOGE(TAG, "MQTT task does not delete");
        return;
    }
    vTaskDelete(*TaskHandler);
    free(xTaskMQTTBuffer);
    free(xMQTTStack);
}

#ifdef MQTT_TEST

/**
 * @brief Test function for MQTT publish and subscribe.
 * This function is used for testing purposes to publish and subscribe to a test topic.
 */
void TestPublishAndSubscribe()
{
    MQTT_Data_str MQTT_Data_1;
    memset(MQTT_Data_1.RawData, 0x0, sizeof(MQTT_Data_1.RawData));
    memset(MQTT_Data_1.Topic, 0x0, sizeof(MQTT_Data_1.Topic));
    strcpy(MQTT_Data_1.Topic, "test/azmon/bib");
    char DataForTest[] = R"EOF(
    This JSON structure includes information about a \n
    hypothetical company, its employees, products, partners, and some \n
    additional metadata. The content has been balanced to approach the target \n
    character count of 2400 characters. You can customize this JSON message \n
    further as needed!
    )EOF";
    strcpy(MQTT_Data_1.RawData, DataForTest);
    // MQTT_Subscribe("test/azmon/bib");
    // MQTT_Subscribe("test/azmon/bib/house");
    // MQTT_Subscribe("test/azmon/bib/test/bibib");
    MQTT_Publish(MQTT_Data_1.Topic, MQTT_Data_1.RawData);
    // MQTT_UnSubscribe(MQTT_Data_1.Topic);
    // MQTT_Disconnect();
}

/**
 * @brief Test function to receive and log MQTT data.
 * This function is used for testing purposes to receive data from the broker and log it.
 */
void test_MQtt()
{
    if (xSemaphoreTake((*MQTT_Interface.MQTT_Config.ConnectedSemaphore), 1) != pdTRUE)
    {
        return;
    }
    char RawData[MQTT_RAW_DATA_LEN];
    memset(RawData, 0x0, MQTT_RAW_DATA_LEN);
    if (xQueueReceive((*MQTT_Interface.MQTT_Config.DataFromBrokerQueue), RawData, 1) == pdTRUE)
        ESP_LOGE(TAG, "DATA is :%s", RawData);
}
#endif
