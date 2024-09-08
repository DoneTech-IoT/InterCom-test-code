#ifndef LVGL_INTERFACE_H_
#define LVGL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include"lvglTimer.h"
#include"GUI_Typedef.h"


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
 * @brief Kills the GUI task and frees allocated memory.
 * This function deletes the GUI task and frees the memory allocated for LVGL components.
 * @param GUITaskHandler Handler of the GUI task to be killed.
 * @return void
 */
void GUI_TaskKill(TaskHandle_t *TaskHandler);



#ifdef __cplusplus
}
#endif

#endif /* LVGL_GUI_H_ */
