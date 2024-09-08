#include "coffeeMaker_GUI.h"

/**
 * @brief Updates the timer display on the GUI with the given time in Milliseconds.
 * @param Milliseconds Duration in Milliseconds to be converted and displayed as minutes and seconds.
 */
void GUI_DisplayUpdateCoffeeMakerTimer(uint16_t Milliseconds)
{
    int totalSeconds = Milliseconds / GUI_SEC;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    char *timeString = (char *)malloc(20 * sizeof(char));
    if (timeString == NULL)
    {
        return;
    }
    snprintf(timeString, 20, "%02d:%02d", minutes, seconds);
    lv_event_send(guider_ui.screen_Timer, LV_EVENT_VALUE_CHANGED, timeString);
}

/**
 * @brief Sends the current count of cups to the GUI.
 * @param NumberOfCups The number of cups to be displayed.
 */
void GUI_DisplayUpdateCupsCounts(uint8_t NumberOfCups)
{
    if (NumberOfCups > 9)
        return;
    char temp[] = "0";
    temp[0] = temp[0] + NumberOfCups;
    lv_event_send(guider_ui.screen_CountOfCup, LV_EVENT_VALUE_CHANGED, temp);
}

/**
 * @brief Controls the visibility of the CoffeeNut image on the GUI.
 * @param OnOrOff If true, the CoffeeNut image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCoffeeBeansIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_CoffeeNutImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_CoffeeNutImage, IMAGE_OFF, NULL);
}

/**
 * @brief Controls the visibility of the Scop image on the GUI.
 * @param OnOrOff If true, the Scop image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCoffeeScopIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_ScopImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_ScopImage, IMAGE_OFF, NULL);
}

/**
 * @brief Controls the visibility of the Tea image on the GUI.
 * @param OnOrOff If true, the Tea image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowTeaLeafIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_TeaImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_TeaImage, IMAGE_OFF, NULL);
}

/**
 * @brief Controls the visibility of the SmallGrind image on the GUI.
 * @param OnOrOff If true, the SmallGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowFineGrindIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_SmallGrindImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_SmallGrindImage, IMAGE_OFF, NULL);
}

/**
 * @brief Controls the visibility of the MediumGrind image on the GUI.
 * @param OnOrOff If true, the MediumGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowMediumGrindIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_MediumGrindImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_MediumGrindImage, IMAGE_OFF, NULL);
}

/**
 * @brief Controls the visibility of the LongGrind image on the GUI.
 * @param OnOrOff If true, the LongGrind image is shown; otherwise, it is hidden.
 */
void GUI_DisplayShowCourseGrindIcon(int OnOrOff)
{
    if (OnOrOff == true)
    {
        lv_event_send(guider_ui.screen_longGrindImage, IMAGE_ON, NULL);
        return;
    }
    lv_event_send(guider_ui.screen_longGrindImage, IMAGE_OFF, NULL);
}

#ifdef GUI_TESTS
/**
 * @brief Runs a sequence of GUI tests, controlling various images on the display.
 * This function toggles the visibility of different images on the GUI in a sequence,
 * with delays in between each toggle.
 */
void GUItest()
{
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC * 5));
    GUI_DisplayShowCoffeeBeansIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC);
    GUI_DisplayUpdateCupsCounts(1);
    GUI_DisplayShowCoffeeBeansIcon(false);

    GUI_DisplayShowCoffeeScopIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC*2);
    GUI_DisplayUpdateCupsCounts(2);
    GUI_DisplayShowCoffeeScopIcon(false);

    GUI_DisplayShowTeaLeafIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC*3);
    GUI_DisplayUpdateCupsCounts(3);
    GUI_DisplayShowTeaLeafIcon(false);

    GUI_DisplayShowFineGrindIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCupsCounts(4);
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC*4);
    GUI_DisplayShowFineGrindIcon(false);

    GUI_DisplayShowMediumGrindIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC*5);
    GUI_DisplayUpdateCupsCounts(5);
    GUI_DisplayShowMediumGrindIcon(false);

    GUI_DisplayShowCourseGrindIcon(true);
    vTaskDelay(pdMS_TO_TICKS(GUI_SEC));
    GUI_DisplayUpdateCoffeeMakerTimer(GUI_SEC*6);
    GUI_DisplayUpdateCupsCounts(6);
    GUI_DisplayShowCourseGrindIcon(false);
}

#endif