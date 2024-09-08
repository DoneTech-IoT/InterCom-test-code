#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManger.h"
#include "Custom_Log.h"
#include "SharedBus.h"
#include "MQTT_Interface.h"
//#include "MatterInterface.h"
//#include "DoneCoffeeMaker.h"

#define CONFIG_DONE_COMPONENT_MQTT
#define TIMER_TIME pdMS_TO_TICKS(500) // in millis
#define TaskStack 5000

QueueHandle_t QueueHandle;
EventGroupHandle_t EventGroupHandle;

char *msg = "HI every one. I'm blackboard.";
char *msg2 = "HI every one. Here we are.";
 SharedBusPacket_t recievePacket; //= {
//     .SourceID = LOG_INTERFACE_ID,
//     .PacketID = 1,
//     .data = msg
// };
// QueueHandle_t MatterBufQueue;
// SemaphoreHandle_t MatterSemaphore = NULL;
// MatterInterfaceHandler_t MatterInterfaceHandler;
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";   
// ****************************** GLobal Functions ****************************** //

// void MatterAttributeUpdateCBMain(callback_type_t type,
//                                 uint16_t endpoint_id, uint32_t cluster_id,
//                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
//                                 void *priv_data)
// {
//     // printf("callback_type_t: %u\n", type);
//     // printf("endpoint_id: %u\n", endpoint_id);
//     // printf("cluster_id: %lu\n", cluster_id);
//     // printf("attribute_id: %lu\n", attribute_id);
//     // printf("val: %p\n", val);
//     // printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
// }

void MatterNetworkConnected()
{
    ESP_LOGI(TAG, "Matter Network Connected\n");
}

/**
 * @brief Function to change colors based on a timer callback
 */
bool start2Send = false;

void vTaskCode1( void * pvParameters )
{        
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
        
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, UI_INTERFACE_ID))
            ESP_LOGE(TAG, "task1-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));        
    }
}

void vTaskCode2( void * pvParameters )
{        
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, MATTER_INTERFACE_ID))
            ESP_LOGE(TAG, "task2-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));

        if(start2Send)
        {
            recievePacket.SourceID = MATTER_INTERFACE_ID;
            recievePacket.PacketID = 1;
            recievePacket.data = msg;
            SharedBusSend(QueueHandle, recievePacket);
            start2Send = false;
        }     
    }       
}

void vTaskCode3( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, MQTT_INTERFACE_ID))
            ESP_LOGE(TAG, "task3-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode4( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_ID))
            ESP_LOGE(TAG, "task4-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode5( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusOne))
            ESP_LOGE(TAG, "task5-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode6( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusTwo))
            ESP_LOGE(TAG, "task6-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode7( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusTree))
            ESP_LOGE(TAG, "task7-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode8( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusFour))
            ESP_LOGE(TAG, "task8-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}

void vTaskCode9( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusFive))
            ESP_LOGE(TAG, "task9-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));   

        if(!start2Send)
            start2Send = true;             
    }
}

void vTaskCode10( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_PlusSix))
            ESP_LOGE(TAG, "task10-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(10));                
    }
}
extern "C" void app_main()
{
    Log_RamOccupy("main", "service manager");
    ServiceMangerTaskInit();
    nvsFlashInit();
    Log_RamOccupy("main", "service manager");

    BaseType_t xReturned;
    TaskHandle_t xHandle1 = NULL;
    TaskHandle_t xHandle2 = NULL;
    TaskHandle_t xHandle3 = NULL;
    TaskHandle_t xHandle4 = NULL;
    TaskHandle_t xHandle5 = NULL;
    TaskHandle_t xHandle6 = NULL;
    TaskHandle_t xHandle7 = NULL;
    TaskHandle_t xHandle8 = NULL;
    TaskHandle_t xHandle9 = NULL;
    TaskHandle_t xHandle10 = NULL;

    SharedBusInit(&EventGroupHandle, &QueueHandle);

//vTaskDelay (pdMS_TO_TICKS(5000));

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCode1,       /* Function that implements the task. */
                    "num1",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle1 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode2,       /* Function that implements the task. */
                    "num2",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle2 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode3,       /* Function that implements the task. */
                    "num3",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle3 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode4,       /* Function that implements the task. */
                    "num4",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle4 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode5,       /* Function that implements the task. */
                    "num5",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle5 );      /* Used to pass out the created task's handle. */                    
    xReturned = xTaskCreate(
                    vTaskCode6,       /* Function that implements the task. */
                    "num6",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle6 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode7,       /* Function that implements the task. */
                    "num7",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle7 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode8,       /* Function that implements the task. */
                    "num8",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle8 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode9,       /* Function that implements the task. */
                    "num9",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle9 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode10,       /* Function that implements the task. */
                    "num10",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle10 );      /* Used to pass out the created task's handle. */                                                                                    
    vTaskDelay(pdMS_TO_TICKS(1000));    

    // if(!start2Send)
    //     start2Send = true;
        
    // recievePacket.SourceID = LOG_INTERFACE_ID;
    // recievePacket.PacketID = 1;
    // recievePacket.data = msg2;
    // SharedBusSend(QueueHandle, recievePacket);

    // Log_RamOccupy("main", "Matter usage");
    // MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    // MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    // MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    // MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    // Matter_TaskInit(&MatterInterfaceHandler);

    //KeyStatePair_t pKeyStatePair;

    while (true)
    {
        // if(xQueueReceive(
        //     *(MatterInterfaceHandler.SharedBufQueue), 
        //     &pKeyStatePair, pdMS_TO_TICKS(1)) == pdTRUE)
        // {
        //     ESP_LOGW(TAG, "pKeyStatePair-> Key: %d, State: %d\n", 
        //     pKeyStatePair.Key, pKeyStatePair.State);        
        // }
        vTaskDelay(100);
    }        
}
