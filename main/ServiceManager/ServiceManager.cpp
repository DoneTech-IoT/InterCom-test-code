#include "ServiceManager.h"
#include "CoffeeMakerApp.hpp"

static const char *TAG = "Service_Manager";

#ifdef CONFIG_DONE_COMPONENT_MATTER
QueueHandle_t MatterBufQueue;
SemaphoreHandle_t MatterSemaphore = NULL;
MatterInterfaceHandler_t MatterInterfaceHandler;
#endif

#ifdef CONFIG_DONE_COMPONENT_MQTT
static QueueHandle_t MQTTDataFromBrokerQueue;
static SemaphoreHandle_t MQTTConnectedSemaphore;
static SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
#endif

#define TASK_LIST_BUFFER_SIZE 512
// #define MONITORING
/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter);

// Static variables for Service Manager task
StaticTask_t *xTaskServiceMangerBuffer;
StackType_t *xServiceMangerStack;

// Global instance of Service Manager
ServiceManger_t ServiceManger;

#ifdef CONFIG_DONE_COMPONENT_MATTER
void MatterAttributeUpdateCBMain(callback_type_t type,
                                 uint16_t endpoint_id, uint32_t cluster_id,
                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
                                 void *priv_data)
{
    printf("callback_type_t: %u\n", type);
    printf("endpoint_id: %u\n", endpoint_id);
    printf("cluster_id: %lu\n", cluster_id);
    printf("attribute_id: %lu\n", attribute_id);
    printf("val: %p\n", val);
    printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
}

void MatterNetworkConnected()
{
    ESP_LOGI(TAG, "Matter Network Connected\n");
}
void MatterServiceRunner()
{
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    Matter_TaskInit(&MatterInterfaceHandler);
}
#endif
#ifdef CONFIG_DONE_COMPONENT_LVGL
/**
 * @brief Creates the GUI task.
 * This function configures and creates the GUI task.
 * @return void
 */
void GUI_TaskCreator()
{
    ServiceManger.tasks[GUI_Task].maximumRAM_Needed = LVGL_STACK * 2;
    strcpy(ServiceManger.tasks[GUI_Task].name, "GUI");
    ServiceManger.tasks[GUI_Task].ramType = PSRAM_;
    ServiceManger.tasks[GUI_Task].startupRAM = GUI_Task;
    ServiceManger.tasks[GUI_Task].TaskCreator = GUI_TaskCreator;
    ServiceManger.tasks[GUI_Task].TaskKiller = GUI_TaskKill;
    ServiceManger.tasks[GUI_Task].taskStack = LVGL_STACK;
    ServiceManger.tasks[GUI_Task].priority = tskIDLE_PRIORITY + 1;
    ServiceManger.tasks[GUI_Task].taskHandler = NULL;
    GUI_TaskInit(&ServiceManger.tasks[GUI_Task].taskHandler, ServiceManger.tasks[GUI_Task].priority, ServiceManger.tasks[GUI_Task].taskStack);
}
#endif

/**
 * @brief Deletes a task.
 * This function deletes the specified task.
 * @param TaskNumber The number of the task to be deleted.
 * @return void
 */
void TaskKiller(int TaskNumber)
{
    ServiceManger.tasks[TaskNumber].TaskKiller(&ServiceManger.tasks[TaskNumber].taskHandler);
    ESP_LOGI(TAG, "Task %d Deleted !", TaskNumber);
}

/**
 * @brief Creates the Spotify task.
 * This function configures and creates the Spotify task.
 * @return void
 */
void SpotifyTaskCreator()
{
    ESP_LOGI(TAG, "TO DO");
}

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceMangerTaskInit()
{
    xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceMangerTask,         // Task function
        "ServiceMangerTask",       // Task name (for debugging)
        SERVICE_MANGER_STACK,      // Stack size (in words)
        NULL,                      // Task parameters (passed to the task function)
        tskIDLE_PRIORITY + 1,      // Task priority (adjust as needed)
        xServiceMangerStack,       // Stack buffer
        xTaskServiceMangerBuffer); // Task control block
}

/**
 * @brief Initializes the Service Manager.
 * This function initializes the Service Manager by creating necessary tasks.
 * @return void
 */
void ServiceMangerInit()
{
    nvsFlashInit();
#ifdef CONFIG_DONE_COMPONENT_LVGL
    GUI_TaskCreator();
    ESP_LOGI(TAG, "GUI Created !");
    vTaskDelay(SERVICE_MANAGER_SEC * 4);
#endif
#ifdef CONFIG_DONE_COMPONENT_SPOTIFY
    SpotifyTaskCreator();
    ESP_LOGI(TAG, "Spotify Created !");
#endif
#ifdef CONFIG_DONE_COMPONENT_MATTER
    MatterServiceRunner();
    ESP_LOGI(TAG, "Matter interface called !");
#else
    ESP_LOGE(TAG, "connect to wifi");
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
#endif
#ifdef CONFIG_DONE_COMPONENT_MQTT_DEFAULT
    MQTT_DefaultConfig(&MQTTDataFromBrokerQueue, &MQTTConnectedSemaphore, &MQTTErrorOrDisconnectSemaphore);
#endif
}

/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter)
{
    ServiceMangerInit();
    CoffeeMakerApplication(&MQTTDataFromBrokerQueue, &MQTTConnectedSemaphore, &MQTTErrorOrDisconnectSemaphore);
    char pcTaskList[TASK_LIST_BUFFER_SIZE];
    while (true)
    {
#ifdef MONITORING
// vTaskList(pcTaskList);
// ESP_LOGI(TAG, "Task List:\n%s\n", pcTaskList);
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}