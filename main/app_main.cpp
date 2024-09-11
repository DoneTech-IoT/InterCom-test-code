#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"

static const char *TAG = "Main";

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{

    Log_RamOccupy("main", "service manager");
    ServiceMangerTaskInit();
    Log_RamOccupy("main", "service manager");
}