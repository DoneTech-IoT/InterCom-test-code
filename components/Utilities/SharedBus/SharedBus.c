#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )
#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )

#define BIT_23	( 1 << 23 ) //prevent overwriting bus for send before one turn of the event loop
#define BIT_22	( 1 << 22 ) //peek to receive switch flag 
#define BIT_21	( 1 << 21 ) //peek to receive switch flag 

static const char *TAG = "SharedBus";

static EventBits_t EventBits;
static EventGroupHandle_t EventGroupHandleLocal;
QueueHandle_t QueueHandle;

esp_err_t SharedBusInit()    
{
    EventBits = 0;
    EventGroupHandleLocal = xEventGroupCreate();  
    QueueHandle = xQueueCreate(1, sizeof(SharedBusPacket_t));     
    return 0;         
}

esp_err_t SharedBusSend(SharedBusPacket_t SharedBusPacket)    
{    
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23,         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdFALSE,        /* Wait for 23th bit, either bit will do. */
                    1);/* Wait a maximum of 100ms for either bit to be set. */
    if(!(EventBits & BIT_23))
    {
        EventBits = xEventGroupSetBits(
                        EventGroupHandleLocal, /* The event group being updated. */
                        BIT_23);               /* The bits being set. */

        //xQueueSend(QueueHandle, &SharedBusPacket, 1);
        xQueueOverwrite(QueueHandle, &SharedBusPacket);        

        EventBits = xEventGroupClearBits(
                        EventGroupHandleLocal,
                        BIT_21);         

        //permission to all task to peek
        EventBits = xEventGroupSetBits(
                        EventGroupHandleLocal, /* The event group being updated. */
                        BIT_22);                               
    }
        
    return false;    
}

esp_err_t SharedBusRecieve(
    SharedBusPacket_t SharedBusPacket,
    TaskInterfaceID_t interfaceID)
{   
    esp_err_t ret = false;
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22 | BIT_21,         /* The bits within the event group to wait for. */
                    pdFALSE,        /* BIT_22 should NOT be cleared before returning. */
                    pdFALSE,         /* Wait for 22th bit, either bit will do. */
                    1);/*           Wait a maximum of 100ms for either bit to be set. */    

    if((EventBits & BIT_22)) //default state
    {           
        ret = true;    
        if(xQueuePeek(QueueHandle, &SharedBusPacket, 1) != pdTRUE)
            ESP_LOGE(TAG, "Failed to peek data from queue.");  

        if (SharedBusPacket.SourceID == interfaceID)
        {
            if((EventBits & BIT_21))
            {
                EventBits = xEventGroupClearBits(
                            EventGroupHandleLocal, /* The event group being updated. */
                            BIT_22);  

                EventBits = xEventGroupClearBits(
                            EventGroupHandleLocal, /* The event group being updated. */
                            BIT_23);              
            }
            else
            {
                //permission to itself
                EventBits = xEventGroupSetBits(
                                EventGroupHandleLocal, /* The event group being updated. */
                                BIT_21);       
            }
            ret = false;    
        }            
    }     
    return ret;
}