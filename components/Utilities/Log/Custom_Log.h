#ifdef __cplusplus
extern "C"
{
#endif
#ifndef _LOG_H_
#define _LOG_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include <esp_event.h>
#include <sys/param.h>
#include <stdint.h>

#define CONFIG_DONE_LOG_RAM
#define CONFIG_DONE_OATH_LOG_RAM
#define CONFIG_DONE_GUI_LOG_RAM
#define CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
#define CONFIG_DONE_MAIN_LOG_RAM

#define LOG_MAX_EVENT 40
#define LOG_MAX_COMPONENT 3
#define STRING_LEN 20
#define ERROR_CODE 100
typedef enum
{
    LogEnd = 0,
    LogStart
} LogFlag;
typedef struct
{
    size_t Psram;
    size_t Sram;
} Ram_srt;
typedef struct
{
    Ram_srt RAM;
    char Name[STRING_LEN];
    size_t TimeStamp;
        uint8_t Counter;
} Event_str;
typedef struct
{
    Event_str Event[LOG_MAX_EVENT];
    char Name[STRING_LEN];
} Component_str;
typedef struct
{
    Component_str Component[LOG_MAX_COMPONENT];
} Log;

/**
 * @brief Occupies RAM for a component and event.
 * This function manages the RAM occupation for a specified component and event,
 * creating new entries or updating existing ones as necessary.
 * @param Component Name of the component.
 * @param EventName Name of the event.
 * @return void
 */
void Log_RamOccupy(char *Component, char *EventName);

/**
 * @brief Logs the status of RAM usage for a component and event.
 * This function logs the current RAM usage and timestamp for a specified component and event.
 * @param ComponentName Name of the component.
 * @param EventName Name of the event.
 * @return void
 */
void Log_RamStatus(char *ComponentName, char *EventName);

/**
 * @brief Reports the RAM usage of a component.
 * This function prints the RAM usage for all events within a specified component.
 * @param ComponentName Name of the component to report.
 * @return void
 */
void Log_ReportComponentRamUsed(char *ComponentName);


/**
 * @brief Reports the RAM status of a component.
 * This function prints the RAM usage status for all events within a specified component.
 * @param ComponentName Name of the component to report.
 * @return void
 */
void Log_ReportComponentRamStatus(char *ComponentName);
#endif


#ifdef __cplusplus
}
#endif



