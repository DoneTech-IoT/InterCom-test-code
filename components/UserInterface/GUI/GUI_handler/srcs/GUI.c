#include"GUI.h"
#include "gui_guider.h"
#include "Custom_Log.h"
#include "SharedBus.h"

static const char *TAG = "GUI";

StaticTask_t *xTaskLVGLBuffer;
StackType_t *xLVGLStack;

lv_color_t *LVGL_BigBuf1;
lv_color_t *LVGL_BigBuf2;

/**
 * @brief Main task for LVGL GUI operations.
 * This function initializes LVGL, sets up the display driver,
 * registers the display driver, sets up the user interface,
 * starts the LVGL timer, and handles LVGL tasks continuously.
 * @param pvParameter Pointer to task parameters (unused).
 * @return void
 */
void GUI_MainTask(void *pvParameter)
{
    SharedBusPacket_t recievePacket;
    char *msg = "HI every one. I'm blackboard.";

    Log_RamOccupy("LVGL", "starting GUI task");
    lv_disp_draw_buf_t disp_draw_buf;
    lv_init();
    lvgl_driver_init();
    lv_disp_draw_buf_init(&disp_draw_buf, LVGL_BigBuf1, LVGL_BigBuf2, LV_HOR_RES_MAX * 100);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &disp_draw_buf;
    lv_disp_drv_register(&disp_drv);
    setup_ui(&guider_ui);
    LVGL_Timer();
    Log_RamOccupy("LVGL", "starting GUI task");

    recievePacket.SourceID = UI_INTERFACE_ID;
    recievePacket.PacketID = 1;
    recievePacket.data = msg;

    while (true)
    {
        SharedBusSend(recievePacket);
      //  ESP_LOGE(TAG, "test sharedBusSend");

        vTaskDelay(pdMS_TO_TICKS(50));
        lv_task_handler();
    }
}

/**
 * @brief Allocates memory for LVGL components.
 * This function allocates memory for LVGL components such as task buffer, task stack,
 * and two big buffers.
 * @param Stack Size of the LVGL task stack.
 * @return true if memory allocation succeeds, false otherwise.
 */
bool GUI_MemoryAllocation(uint32_t Stack)
{
    Log_RamOccupy("LVGL", "allocate memory");
    xTaskLVGLBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xLVGLStack = (StackType_t *)malloc(Stack * sizeof(StackType_t));
    LVGL_BigBuf1 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * GUI_MULTIPLIER_ * sizeof(lv_color_t));
    LVGL_BigBuf2 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * GUI_MULTIPLIER_ * sizeof(lv_color_t));
    if (xTaskLVGLBuffer == NULL || xLVGLStack == NULL || LVGL_BigBuf1 == NULL || LVGL_BigBuf2 == NULL)
    {
        ESP_LOGE(TAG, "Memory allocation failed!");
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
        Log_RamOccupy("LVGL", "allocate memory");
        return false;
    }
    Log_RamOccupy("LVGL", "allocate memory");
    return true;
}

/**
 * @brief Initializes the GUI task.
 * This function initializes the GUI task by allocating memory and creating the task.
 * @param GuiTaskHandler Pointer to the variable that will hold the GUI task handler.
 * @param TaskPriority Priority of the GUI task.
 * @param TaskStack Size of the GUI task stack.
 * @return void
 */
void GUI_TaskInit(TaskHandle_t *GuiTaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack)
{
    bool GUI_MemoryAllocationStatus = GUI_MemoryAllocation(TaskStack);
    if (GUI_MemoryAllocationStatus == false)
    {
        ESP_LOGE(TAG, "GUI task can not be created ");
        return;
    }
    Log_RamStatus("LVGL", "create task");
    Log_RamOccupy("LVGL", "create task");
    *GuiTaskHandler = xTaskCreateStatic(
        GUI_MainTask,   // Task function
        "GUI_MainTask", // Task name (for debugging)
        TaskStack,      // Stack size (in words)
        NULL,           // Task parameters (passed to the task function)
        TaskPriority,   // Task priority (adjust as needed)
        xLVGLStack,     // Stack buffer
        xTaskLVGLBuffer // Task control block
    );
    Log_RamOccupy("LVGL", "create task");
    // this delay so important
    vTaskDelay(GUI_SEC * 0.5);
    ESP_LOGI(TAG, "GUI task successfully created!");
}

/**
 * @brief Kills the GUI task and frees allocated memory.
 * This function deletes the GUI task and frees the memory allocated for LVGL components.
 * @param GUITaskHandler Handler of the GUI task to be killed.
 * @return void
 */
void GUI_TaskKill(TaskHandle_t *TaskHandler)
{
    if (*TaskHandler == NULL)
    {
        ESP_LOGE(TAG, "TaskHandler is NULL !");
        return;
    }
    lv_deinit();
    vTaskDelete(TaskHandler);
    free(xTaskLVGLBuffer);
    free(xLVGLStack);
    free(LVGL_BigBuf2);
    free(LVGL_BigBuf1);
}
