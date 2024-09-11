#ifdef __cplusplus
extern "C"
{
#endif
#ifndef NETWORK_CHECKER_
#define NETWORK_CHECKER_

#include "esp_wifi.h"
#include "esp_log.h"
#include <esp_event.h>
#include "esp_system.h"
#include <sys/param.h>
#include "esp_err.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_http_client.h"

    /**
     * @brief Checks the current Wi-Fi connection status.
     * This function retrieves the information of the currently connected Wi-Fi access point.
     * If connected, it logs the SSID; otherwise, it indicates no connection.
     * @return bool Returns true if connected to a Wi-Fi network, false otherwise.
     */
    bool IsWifiConnected();

    /**
     * @brief Checks the internet connection status.
     * This function attempts to connect to a specified URL to verify if the device has an active internet connection.
     * Logs the connection status and cleans up HTTP client resources.
     * @return bool Returns true if connected to the internet, false otherwise.
     */
    bool CheckInternetConnection();
#endif
#ifdef __cplusplus
}
#endif
