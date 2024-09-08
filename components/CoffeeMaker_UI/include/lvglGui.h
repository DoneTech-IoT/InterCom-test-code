/*
 * a component for create indipendent test for gui
 */
#ifndef LVGL_GUI_H_
#define LVGL_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include"LVGLInterface.h"
#include"lvglTimer.h"

/**
 * @brief Initializes the GUI task.
 * This function initializes the GUI task by allocating memory and creating the task.
 * @param GuiTaskHandler Pointer to the variable that will hold the GUI task handler.
 * @param TaskPriority Priority of the GUI task.
 * @param TaskStack Size of the GUI task stack.
 * @return void
 */
void GUI_TaskInit(TaskHandle_t *GuiTaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack);

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
void GUI_UpdateSpotifyScreen(bool songUpdated, char *Artist, char *Song, char *Album, int DurationMS, int ProgressMS, uint8_t *coverPhoto);

/**
 * @brief Handles the event of Matter network connection.
 * This function is called when the Matter network is connected. It sends an event
 * to update the Matter logo on the LVGL UI.
 * @return void
 */
void MatterNetworkConnected();

/**
 * @brief Kills the GUI task and frees allocated memory.
 * This function deletes the GUI task and frees the memory allocated for LVGL components.
 * @param GUITaskHandler Handler of the GUI task to be killed.
 * @return void
 */
void GUI_TaskKill(TaskHandle_t *TaskHandler);

void Drink_CoffeeBean_Event();
void Drink_CoffeePowder_Event();
void Drink_Tea_Event();

void CoffeeBeanSize_L_Event();
void CoffeeBeanSize_M_Event();
void CoffeeBeanSize_S_Event();

#ifdef __cplusplus
}
#endif

#endif /* LVGL_GUI_H_ */
