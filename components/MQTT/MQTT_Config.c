#include "MQTT_Config.h"
static const char *TAG = "MQTT_Config";

/**
 * @brief Configures the MQTT publisher settings.
 * This function sets up the properties for publishing MQTT messages, such as payload format,
 * message expiry interval, and correlation data.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
void MQTT_PublisherConfiguration(MQTT_Interface_str *MQTT_Interface)
{
    MQTT_Interface->PublishProperty.payload_format_indicator = 1;
    MQTT_Interface->PublishProperty.message_expiry_interval = 1000;
    MQTT_Interface->PublishProperty.topic_alias = 0;
    MQTT_Interface->PublishProperty.response_topic = MQTT_Interface->MQTT_Config.PublishConfig->Topic;
    MQTT_Interface->PublishProperty.correlation_data = "123456";
    MQTT_Interface->PublishProperty.correlation_data_len = 6;
}


/**
 * @brief Configures the MQTT subscriber settings.
 * This function sets up the properties for subscribing to MQTT topics, including subscription ID,
 * flags for local and shared subscriptions, and share name.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
void MQTT_SubscriberConfiguration(MQTT_Interface_str *MQTT_Interface)
{
    MQTT_Interface->SubscribeProperty.subscribe_id =  MQTT_Interface->MQTT_Config.ClientID;
    MQTT_Interface->SubscribeProperty.no_local_flag = false;
    MQTT_Interface->SubscribeProperty.retain_as_published_flag = false;
    MQTT_Interface->SubscribeProperty.retain_handle = 0;
    MQTT_Interface->SubscribeProperty.is_share_subscribe = true;
    MQTT_Interface->SubscribeProperty.share_name = "group1";
}

/**
 * @brief Configures the MQTT disconnection settings.
 * This function sets the properties related to disconnecting from the MQTT broker, such as session
 * expiry interval and disconnect reason.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
static void MQTT_DisconnectConfiguration(MQTT_Interface_str *MQTT_Interface)
{
    MQTT_Interface->DisconnectProperty.session_expiry_interval = 3600;
    MQTT_Interface->DisconnectProperty.disconnect_reason = 0;
}

/**
 * @brief Configures the MQTT unsubscription settings.
 * This function sets the properties for unsubscribing from MQTT topics, including share name and
 * flags for shared subscriptions.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
static void MQTT_UnSubscribeConfiguration(MQTT_Interface_str *MQTT_Interface)
{
    MQTT_Interface->UnsubscribeProperty.is_share_subscribe = true;
    MQTT_Interface->UnsubscribeProperty.share_name = "group1";
}

/**
 * @brief Configures MQTT user properties.
 * This function sets custom user properties for MQTT messages, such as key-value pairs.
 * @param UserPropertyArray An array of user property items to set.
 * @return void
 */
static void MQTT_UserProperty(esp_mqtt5_user_property_item_t *UserPropertyArray)
{
    /* I don't know what is this !*/
    UserPropertyArray[0].key = "board";
    UserPropertyArray[0].value = "esp32";
}

/**
 * @brief Sets up the entire MQTT configuration.
 * This function configures all aspects of the MQTT client, including publisher, subscriber,
 * disconnection settings, and user properties. It initializes the client and starts the MQTT
 * session.
 * @param UserPropertyArray An array of user property items to set.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
void MQTT_ConfigFunction(esp_mqtt5_user_property_item_t *UserPropertyArray, MQTT_Interface_str *MQTT_Interface)
{
    MQTT_PublisherConfiguration(MQTT_Interface);
    MQTT_SubscriberConfiguration(MQTT_Interface);
    MQTT_DisconnectConfiguration(MQTT_Interface);
    MQTT_UnSubscribeConfiguration(MQTT_Interface);
    MQTT_UserProperty(UserPropertyArray);
    esp_mqtt5_connection_property_config_t ConnectProperty = {
        .session_expiry_interval = 10,
        .maximum_packet_size = 1024,
        .receive_maximum = 65535,
        .topic_alias_maximum = 2,
        .request_resp_info = true,
        .request_problem_info = true,
        .will_delay_interval = 10,
        .payload_format_indicator = true,
        .message_expiry_interval = 10,
        .response_topic = "/topic/response",
        .correlation_data = "123456",
        .correlation_data_len = 6,
    };
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = MY_URL,
        .broker.address.port = MY_PORT,
#ifdef MQTT_SSL_ENABLE
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
#endif
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = true,
        .credentials.username = MY_USERNAME,
        .credentials.authentication.password = MY_PASSWORD,

        .session.last_will.msg = "i will leave",
        .session.last_will.msg_len = 12,
        .session.last_will.qos = MQTT_Interface->MQTT_Config.PublishConfig->Qos,
        .session.last_will.retain = true,
        .session.disable_keepalive = false,
        .session.keepalive = 120,
        .session.message_retransmit_timeout = 1,
    };
    MQTT_Interface->MainClient = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt5_client_set_user_property(&ConnectProperty.user_property, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_user_property(&ConnectProperty.will_user_property, UserPropertyArray, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_connect_property(MQTT_Interface->MainClient, &ConnectProperty);
    esp_mqtt5_client_delete_user_property(ConnectProperty.user_property);
    esp_mqtt5_client_delete_user_property(ConnectProperty.will_user_property);
    esp_mqtt_client_register_event(MQTT_Interface->MainClient, ESP_EVENT_ANY_ID, MQTTCore_EventHandler, NULL);
    esp_mqtt_client_start(MQTT_Interface->MainClient);
}
