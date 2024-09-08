#ifndef MATTER_INTERFACE_H
#define MATTER_INTERFACE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_ota.h>
#include <esp_matter_identify.h>
#include <esp_matter_endpoint.h>

#include <app_priv.h>
#include <app_reset.h>
#include <device.h>
#include <led_driver.h>

#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

typedef void (*MatterNetworkEventCBPtr)(const ChipDeviceEvent *event, intptr_t arg);
typedef void (*MatterIdentificationCBPtr)( 
                identification::callback_type_t type, 
                uint16_t endpoint_id, uint8_t effect_id,
                uint8_t effect_variant, void *priv_data);
 typedef void (*MatterAttributeUpdateCBPtr)(
                callback_type_t type, 
                uint16_t endpoint_id, uint32_t cluster_id,
                uint32_t attribute_id, esp_matter_attr_val_t *val, 
                void *priv_data);

typedef void (*UpdateGUI_AddMatterIconPtr)();
typedef struct {
    QueueHandle_t *SharedBufQueue;
    SemaphoreHandle_t *SharedSemaphore;    
    MatterNetworkEventCBPtr MatterNetworkEventCB;
    MatterIdentificationCBPtr MatterIdentificationCB;
    MatterAttributeUpdateCBPtr MatterAttributeUpdateCB;
    UpdateGUI_AddMatterIconPtr ConnectToMatterNetwork;
} MatterInterfaceHandler_t;

bool Matter_TaskInit(MatterInterfaceHandler_t *MatterInterfaceHandler);

#endif //MATTER_INTERFACE_H
