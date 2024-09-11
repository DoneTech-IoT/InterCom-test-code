#include "CoffeeMakerApp.hpp"
#include "coffeeMaker_GUI.h"

static const char *TAG = "coffeeMakerApp";

static uint8_t CoffeeMakerApp_TimerCounter;
TimerHandle_t CoffeeMakerApp_xTimer;

/**
 * @brief Resets the coffee maker GUI to its default state.
 * This function resets the timer counter and clears all display indicators for the coffee maker.
 * @return void
 */
void CoffeeMakerGUIReset()
{
    CoffeeMakerApp_TimerCounter = 0;
    GUI_DisplayUpdateCoffeeMakerTimer(0);
    GUI_DisplayUpdateCupsCounts(0);
    GUI_DisplayShowCoffeeBeansIcon(false);
    GUI_DisplayShowCoffeeScopIcon(false);
    GUI_DisplayShowTeaLeafIcon(false);
    GUI_DisplayShowFineGrindIcon(false);
    GUI_DisplayShowMediumGrindIcon(false);
    GUI_DisplayShowCourseGrindIcon(false);
}

/**
 * @brief Stops the coffee maker timer.
 * This function attempts to stop the timer and resets the GUI upon success or logs an error if it fails.
 * @param CoffeeMakerApp_xTimer Pointer to the timer handle to be stopped.
 * @return void
 */
void CoffeeMakerStopTimer(TimerHandle_t *CoffeeMakerApp_xTimer)
{
    if (xTimerStop(*CoffeeMakerApp_xTimer, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "Timer unsuccessfully stopped please restart this task.\n");
        return;
    }
    ESP_LOGI(TAG, "Timer successfully stopped.\n");
    CoffeeMakerGUIReset();
}

/**
 * @brief Timer callback function for the coffee maker.
 * This function is called when the timer expires, updates the timer counter, and stops the timer when the coffee time is reached.
 * @param CoffeeMakerApp_xTimer Timer handle.
 * @return void
 */
void CoffeeMakerTimerCallBack(TimerHandle_t CoffeeMakerApp_xTimer)
{
    if (CoffeeMakerApp_TimerCounter == COFFEE_TIME)
    {
        CoffeeMakerApp_TimerCounter = 0;
        CoffeeMakerStopTimer(&CoffeeMakerApp_xTimer);
    }
    GUI_DisplayUpdateCoffeeMakerTimer(COFFEE_MAKER_APP_SEC * CoffeeMakerApp_TimerCounter);
    CoffeeMakerApp_TimerCounter++;
}

/**
 * @brief Creates a JSON string for coffee maker settings.
 * This function constructs a JSON string based on the given coffee maker settings and outputs it.
 * @param CoffeeMakerJson Structure containing coffee maker settings.
 * @param CoffeeMakerJsonOutPut Pointer to the output string buffer for the generated JSON.
 * @return void
 */
void CoffeeMakerJsonCreator(CoffeeMakerJson_str CoffeeMakerJson, char *CoffeeMakerJsonOutPut)
{

    cJSON *root = cJSON_CreateObject();
    cJSON_AddBoolToObject(root, "Coffee", CoffeeMakerJson.CoffeeFlag); // true
    cJSON *coffeeProperty = cJSON_CreateObject();
    cJSON_AddNumberToObject(coffeeProperty, "GinderLevel", CoffeeMakerJson.GrinderLevel);
    cJSON_AddNumberToObject(coffeeProperty, "Cup", CoffeeMakerJson.Cups);
    cJSON_AddItemToObject(root, "CoffeeProperty", coffeeProperty);

    cJSON *security = cJSON_CreateObject();
    cJSON_AddStringToObject(security, "Mac", CoffeeMakerJson.DeviceMACAddress);
    cJSON_AddStringToObject(security, "Pass", CoffeeMakerJson.Pass);
    cJSON_AddItemToObject(root, "Security", security);

    cJSON_AddBoolToObject(root, "Tea", 0); // false

    cJSON *teaProperty = cJSON_CreateObject();
    cJSON_AddNumberToObject(teaProperty, "Cups", CoffeeMakerJson.Cups);
    cJSON_AddNumberToObject(teaProperty, "Temp", CoffeeMakerJson.Temp);
    cJSON_AddItemToObject(root, "TeaProperty", teaProperty);

    cJSON_AddNumberToObject(root, "UpdateTime", CoffeeMakerJson.UpdateTime);
    cJSON_AddStringToObject(root, "State", CoffeeMakerJson.State);

    strcpy(CoffeeMakerJsonOutPut, cJSON_Print(root));

    cJSON_Delete(root);
}

/**
 * @brief Parses a JSON string into coffee maker settings.
 * This function reads a JSON input string and populates the provided structure with the parsed data.
 * @param CoffeeMakerJson Pointer to the structure to be filled with parsed data.
 * @param CoffeeMakerJsonInput Input string containing JSON data.
 * @return void
 */
void CoffeeMakerJsonParser(CoffeeMakerJson_str *CoffeeMakerJson, char *CoffeeMakerJsonInput)
{
    cJSON *root = cJSON_Parse(CoffeeMakerJsonInput);
    if (!root)
    {
        ESP_LOGE("json ", "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    cJSON *coffee = cJSON_GetObjectItem(root, "Coffee");
    if (cJSON_IsBool(coffee))
        CoffeeMakerJson->CoffeeFlag = cJSON_IsTrue(coffee) ? 1 : 0;

    cJSON *coffee_property = cJSON_GetObjectItem(root, "CoffeeProperty");
    if (cJSON_IsObject(coffee_property))
    {
        cJSON *ginder_level = cJSON_GetObjectItem(coffee_property, "GinderLevel");
        if (cJSON_IsNumber(ginder_level))
            CoffeeMakerJson->GrinderLevel = (uint8_t)ginder_level->valueint;
    }

    cJSON *security = cJSON_GetObjectItem(root, "Security");
    if (cJSON_IsObject(security))
    {
        cJSON *mac = cJSON_GetObjectItem(security, "Mac");
        cJSON *pass = cJSON_GetObjectItem(security, "Pass");
        if (cJSON_IsString(mac))
            strncpy(CoffeeMakerJson->DeviceMACAddress, mac->valuestring, sizeof(CoffeeMakerJson->DeviceMACAddress) - 1);
        if (cJSON_IsString(pass))
            strncpy(CoffeeMakerJson->Pass, pass->valuestring, sizeof(CoffeeMakerJson->Pass) - 1);
    }

    cJSON *tea = cJSON_GetObjectItem(root, "Tea");
    if (cJSON_IsBool(tea))
        CoffeeMakerJson->TeaFlag = cJSON_IsTrue(tea) ? 1 : 0;

    // Extract and set the "TeaProperty" object
    cJSON *tea_property = cJSON_GetObjectItem(root, "TeaProperty");
    if (cJSON_IsObject(tea_property))
    {
        cJSON *cups = cJSON_GetObjectItem(tea_property, "Cups");
        cJSON *temp = cJSON_GetObjectItem(tea_property, "Temp");
        if (cJSON_IsNumber(cups))
            CoffeeMakerJson->Cups = (uint8_t)cups->valueint;
        if (cJSON_IsNumber(temp))
            CoffeeMakerJson->Temp = (uint8_t)temp->valueint;
    }

    // Extract and set the "UpdateTime" value
    cJSON *update_time = cJSON_GetObjectItem(root, "UpdateTime");
    if (cJSON_IsNumber(update_time))
        CoffeeMakerJson->UpdateTime = (uint16_t)update_time->valueint;

    cJSON *State = cJSON_GetObjectItem(root, "State");
    if (cJSON_IsString(State))
        strncpy(CoffeeMakerJson->State, State->valuestring, sizeof(CoffeeMakerJson->State) - 1);
    cJSON_Delete(root);
}

/**
 * @brief Applies coffee maker settings to the screen.
 * This function updates the GUI based on the current coffee maker settings.
 * @param CoffeeMakerJson Pointer to the structure containing the coffee maker settings.
 * @return void
 */
void ApplyOnScreen(CoffeeMakerJson_str *CoffeeMakerJson)
{
    GUI_DisplayUpdateCupsCounts(CoffeeMakerJson->Cups);
    if (CoffeeMakerJson->CoffeeFlag == true && CoffeeMakerJson->TeaFlag == false)
    {
        GUI_DisplayShowCoffeeBeansIcon(true);
        switch (CoffeeMakerJson->GrinderLevel)
        {
        case 1:
            GUI_DisplayShowCourseGrindIcon(true);
            break;
        case 2:
            GUI_DisplayShowMediumGrindIcon(true);
            break;
        case 3:
            GUI_DisplayShowFineGrindIcon(true);
            break;
        }
    }
    else if (CoffeeMakerJson->TeaFlag == true && CoffeeMakerJson->CoffeeFlag == false)
    {
        GUI_DisplayShowTeaLeafIcon(true);
    }
    if (xTimerStart(CoffeeMakerApp_xTimer, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to start the timer\n");
    }
}

/**
 * @brief Main application function for managing the coffee maker's MQTT communication and user interface.
 * This function initializes the timer for the coffee maker application and handles MQTT subscriptions and messages.
 * It processes incoming data to update the coffee maker's state and display, as well as manages connection status.
 * @param MQTTDataFromBrokerQueue Pointer to the queue handle for receiving MQTT data from the *broker.
 * @param MQTTConnectedSemaphore Pointer to the semaphore handle indicating successful MQTT connection.
 * @param MQTTErrorOrDisconnectSemaphore Pointer to the semaphore handle indicating an MQTT error *or disconnection.
 * @return void
 */
void CoffeeMakerApplication(
    QueueHandle_t *MQTTDataFromBrokerQueue,
    SemaphoreHandle_t *MQTTConnectedSemaphore,
    SemaphoreHandle_t *MQTTErrorOrDisconnectSemaphore)
{
    CoffeeMakerGUIReset();
    CoffeeMakerApp_xTimer = xTimerCreate("Coffee Maker Timer",
                                         pdMS_TO_TICKS(COFFEE_MAKER_APP_SEC),
                                         pdTRUE, (void *)0,
                                         CoffeeMakerTimerCallBack);
    char CoffeeMakerJsonOutPut[2500];
    while (true)
    {
        if (xSemaphoreTake(*MQTTConnectedSemaphore,
                           pdMS_TO_TICKS(COFFEE_MAKER_APP_SEC)) == pdTRUE)
        {
            MQTT_Subscribe("AndroidApp/TV");
#ifdef COFFEE_MAKER_APP_TEST
            PublishJsonForTest(CoffeeMakerJsonOutPut);
#endif
        }
        if (xSemaphoreTake(*MQTTErrorOrDisconnectSemaphore,
                           pdMS_TO_TICKS(COFFEE_MAKER_APP_SEC)) == pdTRUE)
        {
            ESP_LOGE(TAG, "we lose Mqtt");
            break;
        }
        if (xQueueReceive(*MQTTDataFromBrokerQueue,
                          CoffeeMakerJsonOutPut,
                          pdMS_TO_TICKS(COFFEE_MAKER_APP_SEC)) == pdTRUE)
        {
            ESP_LOGE(TAG, "receive data in coffee maker app");
            CoffeeMakerJson_str CoffeeMakerJson;
            CoffeeMakerJsonParser(&CoffeeMakerJson, CoffeeMakerJsonOutPut);
            CoffeeMakerGUIReset();
            ApplyOnScreen(&CoffeeMakerJson);
        }
    }
}

#ifdef COFFEE_MAKER_APP_TEST
void parserTEST(char *temp)
{
    CoffeeMakerJson_str CoffeeMakerJson;
    ESP_LOGI(TAG, "%s", temp);
    CoffeeMakerJsonParser(&CoffeeMakerJson, temp);
    ESP_LOGI(TAG, "Coffee Flag: %u\n", CoffeeMakerJson.CoffeeFlag);
    ESP_LOGI(TAG, "Tea Flag: %u\n", CoffeeMakerJson.TeaFlag);
    ESP_LOGI(TAG, "GrinderLevel: %u\n", CoffeeMakerJson.GrinderLevel);
    ESP_LOGI(TAG, "Cups: %u\n", CoffeeMakerJson.Cups);
    ESP_LOGI(TAG, "Temp: %u\n", CoffeeMakerJson.Temp);
    ESP_LOGI(TAG, "UpdateTime: %u\n", CoffeeMakerJson.UpdateTime);
    ESP_LOGI(TAG, "Device MAC Address: %s\n", CoffeeMakerJson.DeviceMACAddress);
    ESP_LOGI(TAG, "Pass: %s\n", CoffeeMakerJson.Pass);
}
void JSON_TEST(CoffeeMakerJson_str *CoffeeMakerJson)
{

    CoffeeMakerJson->CoffeeFlag = true;
    CoffeeMakerJson->Cups = 4;
    strcpy(CoffeeMakerJson->DeviceMACAddress, "EE:EE:EE:EE:EE:EE");
    strcpy(CoffeeMakerJson->Pass, "12345");
    CoffeeMakerJson->GrinderLevel = 1;
    CoffeeMakerJson->TeaFlag = false;
    CoffeeMakerJson->Temp = 60;
    CoffeeMakerJson->UpdateTime = 60;
    strcpy(CoffeeMakerJson->State, "Ready");

    // parserTEST(CoffeeMakerJsonOutPut);
}
void PublishJsonForTest(char *CoffeeMakerJsonOutPut)
{
    CoffeeMakerJson_str CoffeeMakerJson;
    CoffeeMakerJsonCreator(CoffeeMakerJson, CoffeeMakerJsonOutPut);
    ESP_LOGI("json ", " CoffeeMakerJsonOutPut: %s\n", CoffeeMakerJsonOutPut);
    MQTT_Publish("Device", CoffeeMakerJsonOutPut);
    // memset(CoffeeMakerJsonOutPut, 0x0, sizeof(CoffeeMakerJsonOutPut));
}
#endif