#ifndef SHARED_BUS_H
#define SHARED_BUS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

typedef enum {
    UI_INTERFACE_ID = 1,
    MATTER_INTERFACE_ID = 2,
    MQTT_INTERFACE_ID = 3,  
    LOG_INTERFACE_ID = 4,
    LOG_INTERFACE_PlusOne,
    LOG_INTERFACE_PlusTwo,
    LOG_INTERFACE_PlusTree,
    LOG_INTERFACE_PlusFour,
    LOG_INTERFACE_PlusFive,
    LOG_INTERFACE_PlusSix,
} TaskInterfaceID_t;

typedef struct {
    uint8_t SourceID;
    uint8_t PacketID;    
    void *data;
} SharedBusPacket_t;

esp_err_t SharedBusInit();

esp_err_t SharedBusSend(SharedBusPacket_t SharedBusPacket);

esp_err_t SharedBusRecieve(
    SharedBusPacket_t SharedBusPacket, 
    TaskInterfaceID_t interfaceID);    

#ifdef __cplusplus
}
#endif

#endif //SHARED_BUS_H