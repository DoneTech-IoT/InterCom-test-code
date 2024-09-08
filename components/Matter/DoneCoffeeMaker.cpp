#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_identify.h>
#include <esp_matter_endpoint.h>
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "app_priv.h"
#include "iot_button.h"
#include "Buzzer.h"

static const char *TAG = "DoneCoffeeMaker";
uint16_t PowerKeyEndpointID;
uint16_t CookingModeEndpointID;
uint16_t GrinderEndpointID;
uint16_t CupCounterEndpointID;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

constexpr uint16_t TimerValue[TIMER_COUNT] = {5000, 30000};
static TimerHandle_t TimerHandle[TIMER_COUNT];
static QueueHandle_t *DeviceQueueHandle;
static KeyStatePair_t KeyStatePair;
static CoffeeMakerMatter_str CoffeeMakerJson;

/**
 * @brief init a key and register Press callback
 * @param[in] Handle handler
 * @param[in] GpioPin gpio pin for button
 * @param[in] callbackFunc callback for Press event
 */
static void InitKeyWithPressCallback(
    ButtonHandle_t Handle,
    const int32_t &GpioPin,
    void (*CallbackFunc)(void *button_handle, void *usr_data))
{
    // DOTO: uncomment this lines when find correct.
    //  PinMap(Matter/GUI problem solved whit this comment)

    // button_config_t config = {
    //     .type = BUTTON_TYPE_GPIO,
    //     .gpio_button_config = {
    //         .gpio_num = GpioPin,
    //         .active_level = CONFIG_DONE_KEY_ACTIVE_LEVEL,
    //     }
    // };

    // Handle = iot_button_create(&config);
    // iot_button_register_cb(Handle, BUTTON_PRESS_DOWN, CallbackFunc, NULL);
}

/**
 * @brief read value of an attribute
 * @param[in] EndpointID Endpoint ID of the attribute.
 * @param[in] ClusterID Cluster ID of the attribute.
 * @param[in] AttributeID Attribute ID of the attribute.
 * @param[in] AttrVal Pointer to `esp_matter_attr_val_t`.
 * Use appropriate elements as per the value type.
 */
static void GetAttributeValue(
    const uint16_t &EndpointID,
    const uint32_t &ClusterID,
    const uint32_t &AttributeID,
    esp_matter_attr_val_t *AttrVal)
{
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, EndpointID);
    cluster_t *cluster = cluster::get(endpoint, ClusterID);
    attribute_t *attribute = attribute::get(cluster, AttributeID);
    attribute::get_val(attribute, AttrVal);
}

/**
 * @brief change levelControl current value attr.
 * @param[in] Mode mode of changing current value attr.
 * @param[in] ExplicitCurrentValue get direct value to currentValue Attr.
 * @param[in] EndpointID Attribute ID of the attribute.
 */
static void LevelControlUpdateCurrentValue(
    const uint16_t &EndpointID,
    const LevelControlCurrentValueMode_t &Mode,
    const uint8_t &ExplicitCurrentValue)
{
    esp_matter_attr_val_t valCurrentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valMaxLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valMinLevel = esp_matter_invalid(NULL);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCurrentLevel);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::MaxLevel::Id,
        &valMaxLevel);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::MinLevel::Id,
        &valMinLevel);

    if (Mode == INCREMENT_MODE)
    {
        valCurrentLevel.val.u8 += 1;
        if (valCurrentLevel.val.u8 > valMaxLevel.val.u8)
            valCurrentLevel.val.u8 = valMinLevel.val.u8;
    }
    else if (Mode == DECREMENT_MODE)
    {
        valCurrentLevel.val.u8 -= 1;
        if (valCurrentLevel.val.u8 < valMinLevel.val.u8)
            valCurrentLevel.val.u8 = valMaxLevel.val.u8;
    }
    else if (Mode == EXPLICIT_MODE)
    {
        valCurrentLevel.val.u8 = ExplicitCurrentValue;
    }

    attribute::update(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCurrentLevel);
}

/**
 * @brief get PowerKey endpoint state
 * @param[in] OnOffVal get OnOff sate
 * @param[in] MicroSwitchVal get micro switch state
 * @param[in] OperationVal get operational mode
 */
static void GetPowerKeyState(
    esp_matter_attr_val_t *OnOffVal,
    esp_matter_attr_val_t *MicroSwitchVal,
    esp_matter_attr_val_t *OperationVal)
{
    GetAttributeValue(
        PowerKeyEndpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        OnOffVal);

    GetAttributeValue(
        PowerKeyEndpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        MicroSwitchVal);

    GetAttributeValue(
        PowerKeyEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        OperationVal);
}

/**
 * @brief Power on/off Device
 * @param[in] OnOff true for PowerOn, false for PowerOff
 */
static void PowerOnOff(const bool &OnOff)
{
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);

    onOffVal.val.b = OnOff;
    attribute::update(
        PowerKeyEndpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        &onOffVal);

    LevelControlUpdateCurrentValue(
        PowerKeyEndpointID,
        EXPLICIT_MODE,
        STANDBY_MODE);

    if (OnOff)
    {
        BuzzerPlay(BuzzerEffect_t::ONE_BIZ);
        // TODO:
        // turn OnOff LCD
    }
}

/**
 * @brief callback for all os-timers
 * @param[in] xTimer handle to each timer
 */
void CoffeeMakerTimersCallback(TimerHandle_t xTimer)
{
    uint32_t timerID = (uint32_t)pvTimerGetTimerID(xTimer);

    switch (timerID)
    {
    case MICRO_SWITCH:
        ESP_LOGI(TAG, "MICRO_SWITCH timer callback");
        BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
        break;

    case AUTO_TURN_OFF:
        ESP_LOGI(TAG, "AUTO_TURN_OFF timer callback");
        xTimerStop(TimerHandle[AUTO_TURN_OFF], 0);
        break;
    default:
        break;
    }
}

/**
 * @brief callback for microswitch key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void MicroSwitchCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "MicroSwitchCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    microSwitchVal.val.b = !microSwitchVal.val.b;
    if (microSwitchVal.val.b == ERROR_MODE)
    {
        if (operationVal.val.u8 == ON_MODE)
        {
            ESP_LOGI(TAG, "MicroSwitch Error Interrupt");
            BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);

            LevelControlUpdateCurrentValue(
                PowerKeyEndpointID,
                EXPLICIT_MODE,
                PAUSE_MODE);

            if (xTimerStart(TimerHandle[MICRO_SWITCH], 0) == pdPASS)
            {
                ESP_LOGI(TAG, "MicroSwitch Buzzer Timer Start");
            }
            // TODO: LCD start blink effect
            // TODO: cooking mode stop
        }
    }
    else if (microSwitchVal.val.b == NORMAL_MODE)
    {
        if (operationVal.val.u8 == PAUSE_MODE)
        {
            LevelControlUpdateCurrentValue(
                PowerKeyEndpointID,
                EXPLICIT_MODE,
                ON_MODE);

            xTimerStop(TimerHandle[MICRO_SWITCH], 0);

            // TODO: LCD stop blink effect
            // TODO: cooking mode resume correctly
        }
    }

    attribute::update(
        PowerKeyEndpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &microSwitchVal);
}

/**
 * @brief callback for Power key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void PowerKeyCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "PowerKeyCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t cookingModeVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    GetAttributeValue(
        CookingModeEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &cookingModeVal);

    if (onOffVal.val.b) // when powerON
    {
        if (microSwitchVal.val.b == ERROR_MODE)
        {
            ESP_LOGI(TAG, "MicroSwitch Error");
            BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);

            LevelControlUpdateCurrentValue(
                PowerKeyEndpointID,
                EXPLICIT_MODE,
                PAUSE_MODE);

            KeyStatePair.Key = Keys_t::POWER_KEY;
            KeyStatePair.State = PowerKeyMode_t::PAUSE_MODE;
        }
        else if (microSwitchVal.val.b == NORMAL_MODE)
        {
            LevelControlUpdateCurrentValue(
                PowerKeyEndpointID,
                EXPLICIT_MODE,
                ON_MODE);

            KeyStatePair.Key = Keys_t::POWER_KEY;
            KeyStatePair.State = PowerKeyMode_t::ON_MODE;
            // TODO : //register powerKeyHold callback

            switch (cookingModeVal.val.u8)
            {
            case GRINDER_MODE:
            {
                ESP_LOGI(TAG, "GRINDER_MODE");
                ESP_LOGI(TAG, "TurnOn GrinderMotor Actuator");
                break;
            }
            case COFFEE_MODE:
            {
                ESP_LOGI(TAG, "COFFEE_MODE");
                ESP_LOGI(TAG, "TurnOn Water Actuator");
                ESP_LOGI(TAG, "TurnOn Heater");
                ESP_LOGI(TAG, "TurnOn Timer for Coffee");
                break;
            }
            case TEA_MODE:
            {
                ESP_LOGI(TAG, "TEA_MODE");
                ESP_LOGI(TAG, "TurnOn Water Actuator");
                ESP_LOGI(TAG, "TurnOn Heater");
                ESP_LOGI(TAG, "TurnOn Timer for Tea");
                break;
            }
            default:
                break;
            }
        }
    }
    else // when powerOFF
    {
        ESP_LOGI(TAG, "PowerKey, Power On");
        PowerOnOff(true);
        KeyStatePair.Key = Keys_t::POWER_KEY;
        KeyStatePair.State = PowerKeyMode_t::STANDBY_MODE;
    }

    // xQueueSend(*DeviceQueueHandle, &KeyStatePair, 0);
}

/**
 * @brief callback for Power key LongPress callback
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void PowerKeyLongPressCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "PowerKeyLongPressCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t cookingModeVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    GetAttributeValue(
        CookingModeEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &cookingModeVal);

    if (microSwitchVal.val.b == ERROR_MODE)
    {
        ESP_LOGI(TAG, "MicroSwitch Error");
        BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
    }
    else if (microSwitchVal.val.b == NORMAL_MODE)
    {
        if (operationVal.val.u8 == ON_MODE)
        {
        }
    }
}

/**
 * @brief Power key hardware init
 * @return AppDriverHandle_t (void *)
 */
static AppDriverHandle_t PowerKeyInit()
{
    AppDriverHandle_t handle;
    ButtonHandle_t btnHandle;

    InitKeyWithPressCallback(
        btnHandle,
        CONFIG_DONE_COFFEE_MAKER_POWER_KEY,
        PowerKeyCB);

    iot_button_register_cb(
        btnHandle,
        BUTTON_LONG_PRESS_START,
        PowerKeyLongPressCB, NULL);

    InitKeyWithPressCallback(
        btnHandle,
        CONFIG_DONE_COFFEE_MAKER_MICRO_SWITCH,
        MicroSwitchCB);

    return (AppDriverHandle_t)handle;
}

/**
 * @brief callback for Grinder key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void CookingModeGrindCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeGrindCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t grinderVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    GetAttributeValue(
        GrinderEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &grinderVal);

    if (onOffVal.val.b)
    {
        if (operationVal.val.u8 == STANDBY_MODE)
        {
            ESP_LOGI(TAG, "CookingModeGrindCB STANDBY_MODE");
            LevelControlUpdateCurrentValue(
                GrinderEndpointID,
                INCREMENT_MODE,
                DONT_CARE);

            LevelControlUpdateCurrentValue(
                CookingModeEndpointID,
                EXPLICIT_MODE,
                GRINDER_MODE);

            BuzzerPlay(BuzzerEffect_t::ONE_BIZ);

            KeyStatePair.Key = Keys_t::GRINDER_KEY;
            KeyStatePair.State = grinderVal.val.u8;
        }
        else // any other mode
        {
            if (microSwitchVal.val.b == ERROR_MODE)
            {
                ESP_LOGI(TAG, "CookingModeGrindCB MicroSwitch Error");
                BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
            }
        }
    }
    else // every key can PowerOn device
    {
        ESP_LOGI(TAG, "CookingModeCoffeeCB, Power On");
        PowerOnOff(true);
        KeyStatePair.Key = Keys_t::POWER_KEY;
        KeyStatePair.State = PowerKeyMode_t::STANDBY_MODE;
    }

    // xQueueSend(*DeviceQueueHandle, &KeyStatePair, 0);
}

/**
 * @brief callback for Cooking-Mode Coffee key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void CookingModeCoffeeCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeCoffeeCB ");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    if (onOffVal.val.b)
    {
        if (operationVal.val.u8 == STANDBY_MODE)
        {
            ESP_LOGI(TAG, "CookingModeCoffeeCB STANDBY_MODE");
            LevelControlUpdateCurrentValue(
                CookingModeEndpointID,
                EXPLICIT_MODE,
                COFFEE_MODE);

            BuzzerPlay(BuzzerEffect_t::ONE_BIZ);

            KeyStatePair.Key = Keys_t::COFFEE_KEY;
            KeyStatePair.State = CookingMode_t::DONT_CARE;
        }
        else // any other mode
        {
            if (microSwitchVal.val.b == ERROR_MODE)
            {
                ESP_LOGI(TAG, "CookingModeCoffeeCB MicroSwitch Error");
                BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
            }
        }
    }
    else // every key can PowerOn device
    {
        ESP_LOGI(TAG, "CookingModeCoffeeCB, Power On");
        KeyStatePair.Key = Keys_t::POWER_KEY;
        KeyStatePair.State = PowerKeyMode_t::STANDBY_MODE;
        PowerOnOff(true);
    }
    // xQueueSend(*DeviceQueueHandle, &KeyStatePair, 0);
}

/**
 * @brief callback for Cooking-Mode key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void CookingModeTeaCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeTeaCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    if (onOffVal.val.b)
    {
        if (operationVal.val.u8 == STANDBY_MODE)
        {
            ESP_LOGI(TAG, "CookingModeTeaCB STANDBY_MODE");
            LevelControlUpdateCurrentValue(
                CookingModeEndpointID,
                EXPLICIT_MODE,
                TEA_MODE);

            BuzzerPlay(BuzzerEffect_t::ONE_BIZ);

            KeyStatePair.Key = Keys_t::TEA_KEY;
            KeyStatePair.State = CookingMode_t::DONT_CARE;
        }
        else // any other mode
        {
            if (microSwitchVal.val.b == ERROR_MODE)
            {
                ESP_LOGI(TAG, "CookingModeTeaCB MicroSwitch Error");
                BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
            }
        }
    }
    else // every key can PowerOn device
    {
        ESP_LOGI(TAG, "CookingModeCoffeeCB, Power On");
        PowerOnOff(true);
        KeyStatePair.Key = Keys_t::POWER_KEY;
        KeyStatePair.State = PowerKeyMode_t::STANDBY_MODE;
    }

    // xQueueSend(*DeviceQueueHandle, &KeyStatePair, 0);
}

/**
 * @brief cookingMode key hardware init
 * @return AppDriverHandle_t (void *)
 */
static AppDriverHandle_t CookingModeInit()
{
    AppDriverHandle_t handle;
    ButtonHandle_t btn1Handle, btn2Handle, btn3Handle;

    InitKeyWithPressCallback(
        btn1Handle,
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_GRINDER,
        CookingModeGrindCB);

    InitKeyWithPressCallback(
        btn2Handle,
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_COFFEE,
        CookingModeCoffeeCB);

    InitKeyWithPressCallback(
        btn3Handle,
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_TEA,
        CookingModeTeaCB);

    return (AppDriverHandle_t)handle;
}

/**
 * @brief callback for cupCounter key
 * @param[in] Arg void *button_handle in iot_button callback
 * @param[in] Data  void *usr_data in iot_button callback
 */
static void CupCounterKeyCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CupCounterKeyCB");
    esp_matter_attr_val_t onOffVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t microSwitchVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t operationVal = esp_matter_invalid(NULL);
    esp_matter_attr_val_t cupCounterVal = esp_matter_invalid(NULL);

    GetPowerKeyState(
        &onOffVal,
        &microSwitchVal,
        &operationVal);

    GetAttributeValue(
        CupCounterEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &cupCounterVal);

    if (onOffVal.val.b)
    {
        if (operationVal.val.u8 == STANDBY_MODE)
        {
            ESP_LOGI(TAG, "CupCounterKeyCB");
            LevelControlUpdateCurrentValue(
                CupCounterEndpointID,
                INCREMENT_MODE,
                DONT_CARE);
            BuzzerPlay(BuzzerEffect_t::ONE_BIZ);

            KeyStatePair.Key = Keys_t::CUP_COUNTER_KEY;
            KeyStatePair.State = cupCounterVal.val.u8;
        }
        else // any other mode
        {
            if (microSwitchVal.val.b == ERROR_MODE)
            {
                ESP_LOGI(TAG, "CupCounterKeyCB MicroSwitch Error");
                BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
            }
        }
    }
    else // every key can PowerOn device
    {
        ESP_LOGI(TAG, "CookingModeCoffeeCB, Power On");
        PowerOnOff(true);
        KeyStatePair.Key = Keys_t::POWER_KEY;
        KeyStatePair.State = PowerKeyMode_t::STANDBY_MODE;
    }

    // xQueueSend(*DeviceQueueHandle, &KeyStatePair, 0);
}

/**
 * @brief cupCounter key hardware init
 * @return AppDriverHandle_t (void *)
 */
static AppDriverHandle_t CupCounterInit()
{
    AppDriverHandle_t handle;
    ButtonHandle_t btnHandle;

    InitKeyWithPressCallback(
        btnHandle,
        CONFIG_DONE_COFFEE_MAKER_CUP_COUNTER_KEY,
        CupCounterKeyCB);

    return (AppDriverHandle_t)handle;
}

/**
 * @brief ongoing state, not implemented yet
 */
esp_err_t CookingModeSetEnable(
    AppDriverHandle_t DriverHandle,
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

/**
 * @brief ongoing state, not implemented yet
 */
esp_err_t GrinderSetEnable(
    AppDriverHandle_t DriverHandle,
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

/**
 * @brief ongoing state, not implemented yet
 */
esp_err_t CupCounterSetEnable(
    AppDriverHandle_t DriverHandle,
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

/**
 * @brief This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 * @param[in] EndpointID Endpoint ID of the attribute.
 * @param[in] ClusterID Cluster ID of the attribute.
 * @param[in] AttributeID Attribute ID of the attribute.
 * @param[in] Val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerAttributeUpdate(
    AppDriverHandle_t DriverHandle,
    const uint16_t &EndpointID, const uint32_t &ClusterID,
    const uint32_t &AttributeID, esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    if (EndpointID == PowerKeyEndpointID)
    {
        if (ClusterID == OnOff::Id)
        {
            if (AttributeID == OnOff::Attributes::OnOff::Id)
            {
                ESP_LOGW(TAG, "PowerKeyCB Matter Write");
                // PowerKeyCB(NULL, NULL);
            }
        }
        else if (ClusterID == BooleanState::Id)
        {
            if (AttributeID == BooleanState::Attributes::StateValue::Id)
            {
                ESP_LOGW(TAG, "MicroSwitch Matter Read");
            }
        }
        else if (ClusterID == LevelControl::Id)
        {
            if (AttributeID == LevelControl::Attributes::CurrentLevel::Id)
            {
                ESP_LOGW(TAG, "CookingModeGrindCB Matter Write");
                // CookingModeGrindCB(NULL, NULL);
            }
        }
    }
    else if (EndpointID == CookingModeEndpointID)
    {
        if (ClusterID == OnOff::Id)
        {
            if (AttributeID == OnOff::Attributes::OnOff::Id)
            {
            }
        }
        else if (ClusterID == LevelControl::Id)
        {
            if (AttributeID == LevelControl::Attributes::CurrentLevel::Id)
            {
                if (Val->val.u8 == CookingMode_t::COFFEE_MODE)
                {
                    CoffeeMakerJson.CoffeeFlag = 1;
                    CoffeeMakerJson.TeaFlag = 0;
                }
                else if (Val->val.u8 == CookingMode_t::TEA_MODE)
                {
                    CoffeeMakerJson.CoffeeFlag = 0;
                    CoffeeMakerJson.TeaFlag = 1;
                }
            }
        }
    }
    else if (EndpointID == GrinderEndpointID)
    {
        if (ClusterID == OnOff::Id)
        {
            if (AttributeID == OnOff::Attributes::OnOff::Id)
            {
            }
        }
        else if (ClusterID == LevelControl::Id)
        {
            if (AttributeID == LevelControl::Attributes::CurrentLevel::Id)
            {
                CoffeeMakerJson.GrinderLevel = Val->val.u8;
            }
        }
    }
    else if (EndpointID == CupCounterEndpointID)
    {
        if (ClusterID == OnOff::Id)
        {
            if (AttributeID == OnOff::Attributes::OnOff::Id)
            {
            }
        }
        else if (ClusterID == LevelControl::Id)
        {
            if (AttributeID == LevelControl::Attributes::CurrentLevel::Id)
            {
                CoffeeMakerJson.Cups = Val->val.u8;
            }
        }
    }

    xQueueSend(*DeviceQueueHandle, &CoffeeMakerJson, 0);
    return err;
}

/**
 * @brief create coffee maker device with its related endpoints
 * @param[in] Node Endpoint0 or root Node.
 * @param[in] QueueHandle queue for send event or data of device
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerCreate(
    node_t *Node,
    QueueHandle_t *QueueHandle)
{
    esp_err_t err = ESP_OK;
    DeviceQueueHandle = QueueHandle; // for entire class usage

    AppDriverHandle_t powerKeyHandle = PowerKeyInit();
    DoneMasterPowerKey::config_t powerKeyConfig;
    powerKeyConfig.on_off.on_off = true; // for powerOn when Plug in Powerline.
    powerKeyConfig.boolean.state_value = MicroSwitchMode_t::NORMAL_MODE;
    powerKeyConfig.level_control.current_level = PowerKeyMode_t::STANDBY_MODE;
    powerKeyConfig.level_control.lighting.min_level = 1;
    powerKeyConfig.level_control.lighting.max_level = 3;
    endpoint_t *powerKeyEndpoint = DoneMasterPowerKey::create(
        Node, &powerKeyConfig,
        ENDPOINT_FLAG_NONE, powerKeyHandle);
    if (!powerKeyEndpoint)
    {
        ESP_LOGE(TAG, "powerKeyEndpoint creation failed");
    }
    else
    {
        PowerKeyEndpointID = endpoint::get_id(powerKeyEndpoint);
        ESP_LOGI(TAG, "powerKeyEndpoint created with EndpointID %d", PowerKeyEndpointID);
    }

    AppDriverHandle_t cookingModeHandle = CookingModeInit();
    DoneMultiFunctionSwitch::config_t CookingModeConfig;
    CookingModeConfig.on_off.on_off = true;
    CookingModeConfig.level_control.current_level = CookingMode_t::GRINDER_MODE;
    CookingModeConfig.level_control.lighting.min_level = 1;
    CookingModeConfig.level_control.lighting.max_level = 3;
    endpoint_t *cookingModeEndpoint = DoneMultiFunctionSwitch::create(Node, &CookingModeConfig, ENDPOINT_FLAG_NONE, cookingModeHandle);
    if (!cookingModeEndpoint)
    {
        ESP_LOGE(TAG, "cookingModeEndpoint creation failed");
    }
    else
    {
        CookingModeEndpointID = endpoint::get_id(cookingModeEndpoint);
        ESP_LOGI(TAG, "cookingModeEndpoint created with EndpointID %d", CookingModeEndpointID);
    }

    DoneMultiFunctionSwitch::config_t grinderConfig;
    grinderConfig.on_off.on_off = true;
    grinderConfig.level_control.current_level = 1;
    grinderConfig.level_control.lighting.min_level = 1;
    grinderConfig.level_control.lighting.max_level = 3;
    endpoint_t *grinderEndpoint = DoneMultiFunctionSwitch::create(Node, &grinderConfig, ENDPOINT_FLAG_NONE, NULL);
    if (!grinderEndpoint)
    {
        ESP_LOGE(TAG, "grinderEndpoint creation failed");
    }
    else
    {
        GrinderEndpointID = endpoint::get_id(grinderEndpoint);
        ESP_LOGI(TAG, "grinderEndpoint created with EndpointID %d", GrinderEndpointID);
    }

    AppDriverHandle_t cupCounterHandle = CupCounterInit();
    DoneMultiFunctionSwitch::config_t cupCounterConfig;
    cupCounterConfig.on_off.on_off = true;
    cupCounterConfig.level_control.current_level = 2;
    cupCounterConfig.level_control.lighting.min_level = 2;
    cupCounterConfig.level_control.lighting.max_level = 7;
    endpoint_t *cupCounterEndpoint = DoneMultiFunctionSwitch::create(Node, &cupCounterConfig, ENDPOINT_FLAG_NONE, cupCounterHandle);
    if (!cupCounterEndpoint)
    {
        ESP_LOGE(TAG, "cupCounter_endpoint creation failed");
    }
    else
    {
        CupCounterEndpointID = endpoint::get_id(cupCounterEndpoint);
        ESP_LOGI(TAG, "cupCounterEndpoint created with EndpointID %d", CupCounterEndpointID);
    }

    // create all timer in this device
    for (uint8_t i = 0; i < TIMER_COUNT; i++)
    {
        TimerHandle[i] = xTimerCreate("CoffeeMakerTimers",
                                      pdMS_TO_TICKS(TimerValue[i]),
                                      pdTRUE,    // periodic
                                      (void *)i, // timerID
                                      CoffeeMakerTimersCallback);
    }

    return err;
}