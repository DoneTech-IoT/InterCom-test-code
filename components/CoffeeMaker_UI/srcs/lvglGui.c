#include "lvglGui.h"
#include "gui_guider.h"
#include "Custom_Log.h"

StaticTask_t *xTaskLVGLBuffer;
StackType_t *xLVGLStack;
lv_color_t *LVGL_BigBuf1;
lv_color_t *LVGL_BigBuf2;

static const char *TAG = "LVGL_GUI";
void GUI_mainTask(void *pvParameter);

lv_ui guider_ui;

/**
 * @brief Allocates memory for LVGL components.
 * This function allocates memory for LVGL components such as task buffer, task stack,
 * and two big buffers.
 * @param Stack Size of the LVGL task stack.
 * @return 1 if memory allocation succeeds, 0 otherwise.
 */
uint8_t GUI_MemoryAllocation(uint32_t Stack)
{
    Log_RamOccupy("LVGL", "allocate memory");
    xTaskLVGLBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xLVGLStack = (StackType_t *)malloc(Stack * sizeof(StackType_t));
    LVGL_BigBuf1 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    LVGL_BigBuf2 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    if (xTaskLVGLBuffer == NULL || xLVGLStack == NULL || LVGL_BigBuf1 == NULL || LVGL_BigBuf2 == NULL)
    {
        ESP_LOGE(TAG, "Memory allocation failed!");
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
        return false;
        Log_RamOccupy("LVGL", "allocate memory");
        return 0;
    }
    return true;
    Log_RamOccupy("LVGL", "allocate memory");
    return 1;
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
    uint8_t GUI_MemoryAllocationStatus = GUI_MemoryAllocation(TaskStack);
    if (GUI_MemoryAllocationStatus == false)
    {
        ESP_LOGE(TAG, "GUI task can not be created ");
        return;
    }
    Log_RamStatus("LVGL", "create task");
    Log_RamOccupy("LVGL", "create task");
    *GuiTaskHandler = xTaskCreateStatic(
        GUI_mainTask,   // Task function
        "GUI_mainTask", // Task name (for debugging)
        TaskStack,      // Stack size (in words)
        NULL,           // Task parameters (passed to the task function)
        TaskPriority,   // Task priority (adjust as needed)
        xLVGLStack,     // Stack buffer
        xTaskLVGLBuffer // Task control block
    );
    Log_RamOccupy("LVGL", "create task");
    // this delay so important
    vTaskDelay(500);
    ESP_LOGI(TAG, "GUI task successfully created!");
}

/**
 * @brief Main task for LVGL GUI operations.
 * This function initializes LVGL, sets up the display driver,
 * registers the display driver, sets up the user interface,
 * starts the LVGL timer, and handles LVGL tasks continuously.
 * @param pvParameter Pointer to task parameters (unused).
 * @return void
 */
void GUI_mainTask(void *pvParameter)
{
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
    
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        lv_task_handler();
    }
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
        lv_deinit();
        vTaskDelete(TaskHandler);
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
    }
}

/**
 * @brief Updates the LVGL screen with Spotify information.
 * This function updates various elements on the LVGL screen with Spotify information
 * such as artist name, song title, album name, playback progress, and cover photo.
 * @param songUpdated Flag indicating whether the song information has been updated.
 * @param Artist Artist name.
 * @param Song Title of the song.
 * @param Album Album name.
 * @param DurationMS Duration of the song in milliseconds.
 * @param ProgressMS Current playback progress in milliseconds.
 * @param coverPhoto Pointer to the cover photo image data.
 * @return void
 */
void GUI_UpdateSpotifyScreen(bool songUpdated, char *Artist, char *Song, char *Album, int DurationMS, int ProgressMS, uint8_t *coverPhoto)
{
    // int minutes = ProgressMS / 60000;
    // int second = (ProgressMS % 60000) / 1000;
    // char time[20];
    // sprintf(time, "%d:%d", minutes, second);
    // lv_event_send(guider_ui.Spotify_Page_label_time, LV_EVENT_VALUE_CHANGED, time);

    // if (DurationMS == 0)
    // {
    //     ESP_LOGE(TAG, "Duration is zero");
    //     return;
    // }
    // int progress = (ProgressMS * 100) / DurationMS;
    // lv_event_send(guider_ui.Spotify_Page_bar_progress, LV_EVENT_VALUE_CHANGED, progress);

    // if (!songUpdated)
    // {
    //     return;
    // }
    // Log_RamStatus("LVGL", " before update screen");
    // lv_event_send(guider_ui.Spotify_Page_Artist_name, LV_EVENT_VALUE_CHANGED, Artist);
    // lv_event_send(guider_ui.Spotify_Page_Song_name, LV_EVENT_VALUE_CHANGED, Song);
    // lv_event_send(guider_ui.Spotify_Page_Album_name, LV_EVENT_VALUE_CHANGED, Album);
    // lv_event_send(guider_ui.Matter_logo, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_event_send(guider_ui.Spotify_Page_img_song, LV_EVENT_VALUE_CHANGED, coverPhoto);
    // Log_RamStatus("LVGL", " after update screen");
}

/**
 * @brief Handles the event of Matter network connection.
 * This function is called when the Matter network is connected. It sends an event
 * to update the Matter logo on the LVGL UI.
 * @return void
 */
void MatterNetworkConnected()
{
    //lv_event_send(guider_ui.Matter_logo, LV_EVENT_VALUE_CHANGED, NULL);
}

void Drink_CoffeePowder_Event()
{
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_1, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_6, LV_EVENT_PRESSED, NULL);
}

void Drink_Tea_Event()
{
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_1, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_6, LV_EVENT_PRESSED, NULL);
}

void Drink_CoffeeBean_Event()
{
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_1, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_6, LV_EVENT_PRESSED, NULL);
}

void CoffeeBeanSize_L_Event()
{
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_CLICKED, NULL);
    lv_event_send(guider_ui.screen_img_6, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_1, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_PRESSED, NULL);
}

void CoffeeBeanSize_M_Event()
{
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_CLICKED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_1, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_6, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_PRESSED, NULL);
}

void CoffeeBeanSize_S_Event()
{
    lv_event_send(guider_ui.screen_img_5, LV_EVENT_CLICKED, NULL);
    lv_event_send(guider_ui.screen_img_1, LV_EVENT_CLICKED, NULL);

    lv_event_send(guider_ui.screen_img_6, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_3, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_4, LV_EVENT_PRESSED, NULL);
    lv_event_send(guider_ui.screen_img_2, LV_EVENT_PRESSED, NULL);
}