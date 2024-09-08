#ifndef TASK_MANGER_H_
#define TASK_MANGER_H_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>

#define LVGL_STACK 100 * 1000 // in word not byte
#define SERVICE_MANGER_STACK 5000
#define NUMBER_OF_COSTUME_TASK 10
typedef enum
{
    GUI_Task = 0,
    SpotifyTask,
    MatterTask
} TaskEnum;
typedef enum
{
    PSRAM_,
    SRAM_
} RAM_Types;

typedef void (*TaskCreatorPtr)(void);
typedef void (*TaskKillerPtr)(TaskHandle_t *);
typedef struct
{
    char name[32];            // Task name
    UBaseType_t priority;     // Priority of the task
    uint32_t startupRAM;      // Amount of RAM needed at startup time
    TaskHandle_t taskHandler; // Pointer to the task handler function
    TaskCreatorPtr TaskCreator;
    TaskKillerPtr TaskKiller;
    RAM_Types ramType; // RAM type where task occupies (PSRAM or SRAM)
    uint32_t taskStack;
    uint32_t maximumRAM_Needed; // Maximum SRAM needed by any task
} Task;

typedef struct
{
    Task tasks[NUMBER_OF_COSTUME_TASK]; // Array of tasks (assuming a maximum of 10 tasks)
} ServiceManger_t;

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceMangerTaskInit();
#endif