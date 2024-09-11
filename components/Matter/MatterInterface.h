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

/**
 * @brief Initializes and creates the Matter task.
 * create all matter core components and MatterInterface Task.
 * @param MatterHandle Pointer to the task handle for the created task.
 * @param TaskPriority Priority of the task.
 * @param TaskStack Stack size for the task.
 * @return error if exists
 */
esp_err_t Matter_TaskInit(
    MatterInterfaceHandler_t *MatterInterfaceHandler,
    TaskHandle_t *MatterHandle, UBaseType_t TaskPriority, 
    uint32_t TaskStack);

/**
 * @brief Deletes the Matter task and frees associated resources.
 * This function deletes the specified Matter task and releases the memory allocated for
 * the task stack and control block.
 * @param TaskHandler Pointer to the task handle to be deleted.
 * @return void
 */
void Matter_TaskKill(TaskHandle_t *TaskHandler);
#endif //MATTER_INTERFACE_H
