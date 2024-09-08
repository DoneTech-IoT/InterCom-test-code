/*
 * a component for create independent test for gui
 */
#ifndef GUI_TASK_HANDLER_H_
#define GUI_TASK_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_timer.h"

#include "freertos/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

#include "lvgl.h"
#include "lvgl_helpers.h"
#include "rm67162Lilygo.h"
#include "rm67162.h"

#define GUI_MULTIPLIER_ 2

#define LV_TICK_PERIOD_MS 1
#define TIMER_CALLBACK_TIME 10 * 1000 /* in milliseconds */
#define GUI_SEC 1000



#ifdef __cplusplus
}
#endif

#endif /* GUI_TASK_HANDLER_H_ */
