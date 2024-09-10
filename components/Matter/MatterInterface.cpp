#include <esp_err.h>
#include <esp_log.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/ESP32/OpenthreadLauncher.h>
#endif

#include "MatterInterface.h"
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "Custom_Log.h"
#include "SharedBus.h"

// ****************************** Local Variables
static const char *TAG = "MatterInterface";
static MatterInterfaceHandler_t *InterfaceHandler;
StaticTask_t *MatterTaskBuffer;
StackType_t *MatterStack;
TaskHandle_t MatterHandle = NULL;
#define MATTER_STACK_SIZE 1024 * 4
void MatterTask(void *pvParameter);
// ****************************** Local Functions
using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

/** 
 * @brief Matter App. event callback
 * @param[in] event Matter stack internal event.
 */
static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type)
    {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
    {
        chip::CommissioningWindowManager &commissionMgr = chip::Server::GetInstance().GetCommissioningWindowManager();
        if (!commissionMgr.IsCommissioningWindowOpen())
        {
            if (InterfaceHandler->ConnectToMatterNetwork != NULL)
                InterfaceHandler->ConnectToMatterNetwork();
        }
        Log_RamStatus("Matter", "Interface IP Address Changed");
        ESP_LOGW(TAG, "Interface IP Address changed");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGW(TAG, "Commissioning complete");
        Log_RamStatus("Matter", "Commissioning complete");
        break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
        ESP_LOGW(TAG, "Commissioning failed, fail safe timer expired");
        Log_RamStatus("Matter", "Commissioning failed");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        Log_RamStatus("Matter", "Commissioning started");
        ESP_LOGW(TAG, "Commissioning session started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        ESP_LOGW(TAG, "Commissioning session stopped");
        Log_RamStatus("Matter", "Commissioning started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        Log_RamStatus("Matter", "Commissioning window opened");
        ESP_LOGW(TAG, "Commissioning window opened");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        Log_RamStatus("Matter", "Commissioning window closed");
        ESP_LOGW(TAG, "Commissioning window closed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricRemoved:
        Log_RamStatus("Matter", "Fabric removed successfully");
        ESP_LOGW(TAG, "Fabric removed successfully");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricWillBeRemoved:
        Log_RamStatus("Matter", "Fabric will be removed");
        ESP_LOGW(TAG, "Fabric will be removed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricUpdated:
        Log_RamStatus("Matter", "Fabric is updated");
        ESP_LOGW(TAG, "Fabric is updated");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricCommitted:
        Log_RamStatus("Matter", "Fabric is committed");
        ESP_LOGW(TAG, "Fabric is committed");
        InterfaceHandler->ConnectToMatterNetwork();
        break;

    case chip::DeviceLayer::DeviceEventType::kBLEDeinitialized:
        Log_RamStatus("Matter", "BLE deinitialized");
        ESP_LOGW(TAG, "BLE deinitialized and memory reclaimed");
        break;
    default:
        break;
    }
}

/** 
 * @brief Matter identification callback
 * @param[in] type identification callback type
 * @param[in] endpoint_id endpoint id
 * @param[in] effect_id 
 * @param[in] effect_variant 
 * @param[in] priv_data 
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
static esp_err_t app_identification_cb(
    identification::callback_type_t type, uint16_t endpoint_id, 
    uint8_t effect_id,uint8_t effect_variant, void *priv_data)
{
    ESP_LOGI(TAG, "Identification callback: type: %u, effect: %u, variant: %u", type, effect_id, effect_variant);

    InterfaceHandler->MatterIdentificationCB(type, endpoint_id, effect_id, effect_variant, priv_data);

    return ESP_OK;
}

/**
 * @brief This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
static esp_err_t app_attribute_update_cb(callback_type_t type, 
    uint16_t endpoint_id, uint32_t cluster_id,                                         
    uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
     esp_err_t err = ESP_OK;

    if (type == PRE_UPDATE)//update before save in database(usually drivers)
    {
        AppDriverHandle_t driver_handle = (AppDriverHandle_t)priv_data;
        
#ifdef CONFIG_DONE_MATTER_DEVICE_COFFEE_MAKER            
        err = DoneCoffeeMakerAttributeUpdate(
                driver_handle, endpoint_id, 
                cluster_id, attribute_id, val);
#endif                
    }
    if (type == POST_UPDATE) 
    {
        //TODO: update after save in database
    }    

    InterfaceHandler->MatterAttributeUpdateCB(type, endpoint_id, cluster_id, attribute_id, val, priv_data);
    
    return err;  
}

/** 
 * @brief this API call from app_main to create main task of Matter
 * @param[in] MatterInterfaceHandler Matter task handler
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
bool Matter_TaskInit(MatterInterfaceHandler_t *MatterInterfaceHandler)
{
    InterfaceHandler = MatterInterfaceHandler;

    if (InterfaceHandler->SharedBufQueue != NULL &&
        InterfaceHandler->SharedSemaphore != NULL &&
        // InterfaceHandler->MatterNetworkEventCB != NULL &&
        // InterfaceHandler->MatterIdentificationCB != NULL &&
        InterfaceHandler->MatterAttributeUpdateCB != NULL)
    {
        //*(InterfaceHandler->SharedBufQueue) = xQueueCreate(1, sizeof(KeyStatePair_t));        
        *(InterfaceHandler->SharedBufQueue) = xQueueCreate(1, sizeof(CoffeeMakerMatter_str));
        Log_RamStatus("Matter", "Start Matter");
        esp_err_t err = ESP_OK;        
        
        /* Create a Matter node and add the mandatory Root Node device type on endpoint 0 */
        Log_RamOccupy("Matter", "making node");
        node::config_t node_config;
        node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
        Log_RamOccupy("Matter", "making node");        
        if (!node)
        {
            ESP_LOGE(TAG, "Matter node creation failed");
        }         

#ifdef CONFIG_DONE_MATTER_DEVICE_COFFEE_MAKER            
        Log_RamOccupy("Matter", "making endpoint");        
        err = DoneCoffeeMakerCreate(node, InterfaceHandler->SharedBufQueue);        
#endif                   
               
#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
        /* Set OpenThread platform config */
        esp_openthread_platform_config_t config = {
            .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
            .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
            .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
        };
        set_openthread_platform_config(&config);
#endif

        /* Matter start */
        Log_RamOccupy("Matter", "Matter start");
        err = esp_matter::start(app_event_cb);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Matter start failed: %d", err);
        }
        Log_RamOccupy("Matter", "Matter start");
#if CONFIG_ENABLE_CHIP_SHELL
        Log_RamOccupy("Matter", "Matter init peripheral");
        esp_matter::console::diagnostics_register_commands();
        esp_matter::console::wifi_register_commands();
        esp_matter::console::init();
        Log_RamOccupy("Matter", "Matter init peripheral");
#endif

        ESP_LOGI(TAG, "Matter app initiated successfully");        
ESP_LOGE(TAG, "test2");
        MatterTaskBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
        MatterStack = (StackType_t *)malloc(MATTER_STACK_SIZE * sizeof(StackType_t));
        MatterHandle = xTaskCreateStatic(
                            MatterTask,       /* Function that implements the task. */
                            "MatterTask",          /* Text name for the task. */
                            MATTER_STACK_SIZE,      /* Number of indexes in the xStack array. */
                            NULL,    /* Parameter passed into the task. */
                            tskIDLE_PRIORITY + 1,/* Priority at which the task is created. */
                            MatterStack,          /* Array to use as the task's stack. */
                            MatterTaskBuffer);  /* Variable to hold the task's data structure. */
    }
    else
    {
        ESP_LOGW(TAG, "Matter is already initiated");
    }
    return ESP_OK;
}

void MatterTask(void *pvParameter)
{
    ESP_LOGE(TAG, "test1");
    SharedBusPacket_t recievedPacket;
    while (true)
    {
        ESP_LOGE(TAG, "Packet recieved test");
        if(SharedBusRecieve(recievedPacket, MATTER_INTERFACE_ID))
            ESP_LOGE(TAG, "Packet recieved: %s", (char*) recievedPacket.data);
        else
            ESP_LOGE(TAG, "Failed to recieve.");
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}