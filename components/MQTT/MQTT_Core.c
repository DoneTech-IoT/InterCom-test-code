#include "MQTT_Core.h"

static const char *TAG = "MQTT_core";
MQTT_ThreadConnection_str *MQTT_ThreadConnectionPt;

/**
 * @brief Creates a connection structure for MQTT threads.
 * This function initializes the MQTT thread connection structure, ensuring that all necessary
 * semaphores and queues are available for managing the MQTT connection.
 * @param MQTT_ThreadConnectionInput A pointer to the input structure for the MQTT thread connection.
 * @return esp_err_t ESP_OK if the connection is successfully set up, otherwise ESP_FAIL.
 */
esp_err_t MQTTCore_ThreadConnectionMaker(MQTT_ThreadConnection_str *MQTT_ThreadConnectionInput)
{
    MQTT_ThreadConnectionPt = MQTT_ThreadConnectionInput;

    if (MQTT_ThreadConnectionPt->MQTT_ConnectedSemaphore != NULL &&
        MQTT_ThreadConnectionPt->MQTT_DisconnectedSemaphore != NULL &&
        MQTT_ThreadConnectionPt->MQTT_BufQueue != NULL &&
        MQTT_ThreadConnectionPt->MQTT_BrokerDataSemaphore != NULL)
    {
        return ESP_OK;
    }
    return ESP_FAIL;
}

/**
 * @brief Logs an error message if the given error code is non-zero.
 * This function checks if an error occurred based on the provided error code and logs the
 * corresponding message along with the error code.
 * @param Message The error message to log.
 * @param ErrorCode The error code to check.
 * @return void
 */
void MQTTCore_LogErrorInCore(const char *Message, int ErrorCode)
{
    if (ErrorCode != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", Message, ErrorCode);
    }
}

/**
 * @brief Publishes an MQTT message.
 * This function sends a message to the specified topic with the given properties and user properties.
 * @param Topic The topic to publish to.
 * @param PublishData The data to publish.
 * @param Client A pointer to the MQTT client handle.
 * @param UserProperty A pointer to the user property handle.
 * @param PublishProperty A pointer to the publish property configuration structure.
 * @param UserPropertyArray An array of user property items.
 * @param QOS The Quality of Service level for the message.
 * @param Retain Whether the message should be retained by the broker.
 * @return int The message ID assigned to the published message.
 */
int MQTTCore_Publisher(char *Topic, char *PublishData,
                       esp_mqtt_client_handle_t *Client,
                       mqtt5_user_property_handle_t *UserProperty,
                       esp_mqtt5_publish_property_config_t *PublishProperty,
                       esp_mqtt5_user_property_item_t *UserPropertyArray,
                       int QOS, int Retain)
{
    ESP_LOGW(TAG, "MQTTCore_Publisher run");
    int massageID = 0;
    esp_mqtt5_client_set_user_property(UserProperty, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_publish_property(*Client, &PublishProperty);
    massageID = esp_mqtt_client_publish(*Client, Topic, PublishData, 0, QOS, Retain);
    esp_mqtt5_client_delete_user_property(PublishProperty->user_property);
    PublishProperty->user_property = NULL;
    ESP_LOGW(TAG, "sent publish successful, msg_id=%d", massageID);
    return massageID;
}

/**
 * @brief Subscribes to an MQTT topic.
 * This function subscribes to the specified topic with the given properties and user properties.
 * @param Topic The topic to subscribe to.
 * @param Client A pointer to the MQTT client handle.
 * @param SubscribeProperty A pointer to the subscribe property configuration structure.
 * @param UserPropertyArray An array of user property items.
 * @param UserProperty A pointer to the user property handle.
 * @return int The message ID assigned to the subscription.
 */
int MQTTCore_Subscriber(char *Topic, esp_mqtt_client_handle_t *Client,
                        esp_mqtt5_subscribe_property_config_t *SubscribeProperty,
                        esp_mqtt5_user_property_item_t *UserPropertyArray,
                        mqtt5_user_property_handle_t *UserProperty)
{
    int massageID = 0;
    esp_mqtt5_client_set_user_property(UserProperty, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_subscribe_property(*Client, SubscribeProperty);
    massageID = esp_mqtt_client_subscribe(*Client, Topic, 0);
    esp_mqtt5_client_delete_user_property(SubscribeProperty->user_property);
    SubscribeProperty->user_property = NULL;
    ESP_LOGW(TAG, "sent subscribe successful, msg_id=%d", massageID);
    return massageID;
}

/**
 * @brief Unsubscribes from an MQTT topic.
 * This function unsubscribes from the specified topic with the given properties and user properties.
 * @param Topic The topic to unsubscribe from.
 * @param Client A pointer to the MQTT client handle.
 * @param UserPropertyArray An array of user property items.
 * @param UnsubscribeProperty A pointer to the unsubscribe property configuration structure.
 * @param UserProperty A pointer to the user property handle.
 * @return int The message ID assigned to the unsubscription.
 */
int MQTTCore_Unsubscribe(char *Topic, esp_mqtt_client_handle_t *Client,
                         esp_mqtt5_user_property_item_t *UserPropertyArray,
                         esp_mqtt5_unsubscribe_property_config_t *UnsubscribeProperty,
                         mqtt5_user_property_handle_t *UserProperty)
{
    int massageID = 0;
    esp_mqtt5_client_set_user_property(UserProperty, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_unsubscribe_property(*Client, UnsubscribeProperty);
    massageID = esp_mqtt_client_unsubscribe(*Client, (const char *)Topic);
    ESP_LOGW(TAG, "sent unsubscribe successful, msg_id=%d", massageID);
    esp_mqtt5_client_delete_user_property(UnsubscribeProperty->user_property);
    UnsubscribeProperty->user_property = NULL;
    return massageID;
}

/**
 * @brief Disconnects the MQTT client.
 * This function disconnects the MQTT client from the broker, using the provided properties and user properties.
 * @param Client A pointer to the MQTT client handle.
 * @param UserPropertyArray An array of user property items.
 * @param DisconnectProperty A pointer to the disconnect property configuration structure.
 * @param UserProperty A pointer to the user property handle.
 * @return void
 */
void MQTTCore_Disconnect(esp_mqtt_client_handle_t *Client,
                         esp_mqtt5_user_property_item_t *UserPropertyArray,
                         esp_mqtt5_disconnect_property_config_t *DisconnectProperty,
                         mqtt5_user_property_handle_t *UserProperty)
{
    esp_mqtt5_client_set_user_property(UserProperty, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_disconnect_property(*Client, DisconnectProperty);
    esp_mqtt5_client_delete_user_property(DisconnectProperty->user_property);
    DisconnectProperty->user_property = NULL;
    esp_mqtt_client_disconnect(*Client);
}

/**
 * @brief Deletes MQTT user properties.
 * This function removes user properties from the MQTT client, freeing any allocated memory.
 * @param UserProperty A pointer to the user property handle.
 * @param UnsubscribeProperty A pointer to the unsubscribe property configuration structure.
 * @return void
 */
void MQTTCore_DeleteUserProperty(mqtt5_user_property_handle_t *UserProperty,
                                 esp_mqtt5_unsubscribe_property_config_t *UnsubscribeProperty)
{
    esp_mqtt5_client_delete_user_property(UnsubscribeProperty->user_property);
    esp_mqtt5_client_delete_user_property(*UserProperty);
    UnsubscribeProperty->user_property = NULL;
}

/**
 * @brief Handles MQTT events.
 * This function is registered as the event handler for MQTT events and processes various events
 * like connection, disconnection, data reception, and errors.
 * @param handler_args User data registered to the event.
 * @param Base Event base for the handler.
 * @param EventID The ID of the received event.
 * @param event_data The data associated with the event.
 * @return void
 */
void MQTTCore_EventHandler(void *handler_args, esp_event_base_t Base, int32_t EventID, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop Base=%s, EventID=%" PRIi32, Base, EventID);
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    size_t Time = 0;
    ESP_LOGD(TAG, "free heap size is %" PRIu32 ", minimum %" PRIu32, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());

    switch ((esp_mqtt_event_id_t)EventID)
    {
    case MQTT_EVENT_CONNECTED:
    {
        xSemaphoreGive(MQTT_ThreadConnectionPt->MQTT_ConnectedSemaphore);
        break;
    }
    case MQTT_EVENT_DISCONNECTED:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_DISCONNECTED");
        xSemaphoreGive(MQTT_ThreadConnectionPt->MQTT_DisconnectedSemaphore);
        break;
    }
    case MQTT_EVENT_SUBSCRIBED:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_SUBSCRIBED");
        break;
    }
    case MQTT_EVENT_UNSUBSCRIBED:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_UNSUBSCRIBED");
        break;
    }
    case MQTT_EVENT_PUBLISHED:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_PUBLISHED");
        break;
    }
    case MQTT_EVENT_DATA:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_DATA");
        if (event->data_len != 0)
        {
            MQTT_Data_str MQTT_Data;
            memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
            memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
            memcpy(MQTT_Data.RawData, event->data, event->data_len);
            memcpy(MQTT_Data.Topic, event->topic, event->topic_len);
            xQueueSend(MQTT_ThreadConnectionPt->MQTT_BufQueue, &MQTT_Data, pdMS_TO_TICKS(MQTT_SEC / 3));
            xSemaphoreGive(MQTT_ThreadConnectionPt->MQTT_BrokerDataSemaphore);
            ESP_LOGW(TAG, "send data from core");
        }
        break;
    }
    case MQTT_EVENT_ERROR:
    {
        ESP_LOGW(TAG, "MQTT_EVENT_ERROR");
        ESP_LOGW(TAG, "MQTT5 return code is %d", event->error_handle->connect_return_code);
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            MQTTCore_LogErrorInCore("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            MQTTCore_LogErrorInCore("reported from tls stack", event->error_handle->esp_tls_stack_err);
            MQTTCore_LogErrorInCore("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGW(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        xSemaphoreGive(MQTT_ThreadConnectionPt->MQTT_ErrorInCoreSemaphore);
        break;
    }
    default:
    {
        Time = pdTICKS_TO_MS(xTaskGetTickCount());
        ESP_LOGW(TAG, "Other event id:%d, time from  boot up = %d mil sec", event->event_id, Time);
        break;
    }
    }
}