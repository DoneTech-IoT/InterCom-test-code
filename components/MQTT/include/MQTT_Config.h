#ifdef __cplusplus
extern "C"
{
#endif
#ifndef MQTT_CONFIG_H_
#define MQTT_CONFIG_H_

#include "MQTT_Typedef.h"
#include"MQTT_Core.h"

#define HIVMQ_ENABLE
// #define IRANIAN_BROKER_ENABLE
// #define COREFLUX_ENABLE

#ifdef HIVMQ_ENABLE
#define MY_URL "mqtts://a1ac8d2cf85949888902fda264117496.s1.eu.hivemq.cloud"
#define MY_PORT 8883
#define MY_USERNAME "mohammad_j76"
#define MY_PASSWORD "MohammadJavadAbbasi@76"
#endif

#ifdef COREFLUX_ENABLE
#define MY_URL "mqtts://broker-0d6b761d61.44e20154-733a-4e45-b5e0-ccf9afffa631.blr1.coreflux.cloud"
#define MY_PORT 8883
#define MY_USERNAME "admin"
#define MY_PASSWORD "fkc6CayecC2278hY"
#endif

#ifdef IRANIAN_BROKER_ENABLE
#define MY_URL "mqtt://services.irn6.chabokan.net"
#define MY_PORT 37611
#define MY_USERNAME "erin"
#define MY_PASSWORD "pSiWxkoi3Uy8GV3k"
#endif


#ifndef IRANIAN_BROKER_ENABLE
#define MQTT_SSL_ENABLE
#endif

#define TRY_TO_CONNECT 20
#define BUFF_SIZE 2500


/**
 * @brief Sets up the entire MQTT configuration.
 * This function configures all aspects of the MQTT client, including publisher, subscriber,
 * disconnection settings, and user properties. It initializes the client and starts the MQTT
 * session.
 * @param UserPropertyArray An array of user property items to set.
 * @param MQTT_Interface A pointer to the MQTT interface structure containing configuration details.
 * @return void
 */
void MQTT_ConfigFunction(esp_mqtt5_user_property_item_t *UserPropertyArray, MQTT_Interface_str *MQTT_Interface);



#endif
#ifdef __cplusplus
}
#endif