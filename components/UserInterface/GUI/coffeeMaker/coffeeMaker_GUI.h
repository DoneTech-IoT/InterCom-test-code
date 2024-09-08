

#ifdef __cplusplus
extern "C" {
#endif

#ifndef COFFEE_MAKER_GUI_H_
#define COFFEE_MAKER_GUI_H_

#include "GUI_Typedef.h"
#include "gui_guider.h"
#include "Custom_Log.h"

#define IMAGE_ON LV_EVENT_CLICKED
#define IMAGE_OFF LV_EVENT_PRESSED


/**
 * @brief Updates the timer display on the GUI with the given time in Milliseconds.
 * @param Milliseconds Duration in Milliseconds to be converted and displayed as minutes and seconds.
 */
void GUI_DisplayUpdateCoffeeMakerTimer(uint16_t Milliseconds);

/**
 * @brief Sends the current count of cups to the GUI.
 * @param NumberOfCups The number of cups to be displayed.
 */
void GUI_DisplayUpdateCupsCounts(uint8_t NumberOfCups);

/**
 * @brief Controls the visibility of the CoffeeNut image on the GUI.
 * @param OnOrOff If true, the CoffeeNut image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCoffeeBeansIcon(int OnOrOff);

/**
 * @brief Controls the visibility of the Scop image on the GUI.
 * @param OnOrOff If true, the Scop image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCoffeeScopIcon(int OnOrOff);

/**
 * @brief Controls the visibility of the Tea image on the GUI.
 * @param OnOrOff If true, the Tea image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowTeaLeafIcon(int OnOrOff);

/**
 * @brief Controls the visibility of the SmallGrind image on the GUI.
 * @param OnOrOff If true, the SmallGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowFineGrindIcon(int OnOrOff);

/**
 * @brief Controls the visibility of the MediumGrind image on the GUI.
 * @param OnOrOff If true, the MediumGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowMediumGrindIcon(int OnOrOff);

/**
 * @brief Controls the visibility of the LongGrind image on the GUI.
 * @param OnOrOff If true, the LongGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCourseGrindIcon(int OnOrOff);

#ifdef GUI_TESTS
/**
 * @brief Runs a sequence of GUI tests, controlling various images on the display.
 * This function toggles the visibility of different images on the GUI in a sequence,
 * with delays in between each toggle.
 */
void GUItest();
#endif
#ifdef __cplusplus
}
#endif
#endif
