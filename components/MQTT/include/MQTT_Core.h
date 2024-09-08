#ifdef __cplusplus
extern "C"
{
#endif
#ifndef MQTTCore_INTERFACE_H_
#define MQTTCore_INTERFACE_H_

#include "MQTT_Typedef.h"

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
void MQTTCore_EventHandler(void *handler_args, esp_event_base_t Base, int32_t EventID, void *event_data);

/**
 * @brief Deletes MQTT user properties.
 * This function removes user properties from the MQTT client, freeing any allocated memory.
 * @param UserProperty A pointer to the user property handle.
 * @param UnsubscribeProperty A pointer to the unsubscribe property configuration structure.
 * @return void
 */
void MQTTCore_DeleteUserProperty(mqtt5_user_property_handle_t *UserProperty,
                                 esp_mqtt5_unsubscribe_property_config_t *UnsubscribeProperty);

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
                     mqtt5_user_property_handle_t *UserProperty);

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
                         mqtt5_user_property_handle_t *UserProperty);

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
                        mqtt5_user_property_handle_t *UserProperty);

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
                       int QOS, int Retain);
/**
 * @brief Creates a connection structure for MQTT threads.
 * This function initializes the MQTT thread connection structure, ensuring that all necessary
 * semaphores and queues are available for managing the MQTT connection.
 * @param MQTTCore_ThreadConnectionInput A pointer to the input structure for the MQTT thread connection.
 * @return esp_err_t ESP_OK if the connection is successfully set up, otherwise ESP_FAIL.
 */
esp_err_t MQTTCore_ThreadConnectionMaker(MQTT_ThreadConnection_str *MQTTCore_ThreadConnectionInput);



#endif
#ifdef __cplusplus
}
#endif