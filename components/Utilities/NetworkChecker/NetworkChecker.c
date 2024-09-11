#include "NetworkChecker.h"

static const char *TAG = "Utility_Wifi";

/**
 * @brief Checks the current Wi-Fi connection status.
 * This function retrieves the information of the currently connected Wi-Fi access point.
 * If connected, it logs the SSID; otherwise, it indicates no connection.
 * @return bool Returns true if connected to a Wi-Fi network, false otherwise.
 */
bool IsWifiConnected()
{
    wifi_ap_record_t ap_info;
    esp_err_t status = esp_wifi_sta_get_ap_info(&ap_info);
    if (status != ESP_OK)
    {
        ESP_LOGE(TAG, "Not connected to any Wi-Fi");
        return false;
    }
    ESP_LOGI(TAG, "Connected to SSID: %s", ap_info.ssid);
    return true;
}

/**
 * @brief Event handler for HTTP client events.
 * This function handles various HTTP client events such as errors, receiving headers, etc.
 * Currently, it does nothing and always returns ESP_OK.
 * @param evt Pointer to an HTTP client event structure.
 * @return esp_err_t Returns ESP_OK indicating success.
 */
esp_err_t HttpEventHandler(esp_http_client_event_t *evt)
{
    return ESP_OK;
}

/**
 * @brief Checks the internet connection status.
 * This function attempts to connect to a specified URL to verify if the device has an active internet connection.
 * Logs the connection status and cleans up HTTP client resources.
 * @return bool Returns true if connected to the internet, false otherwise.
 */
bool CheckInternetConnection()
{
    esp_http_client_config_t config = {
        .url = "http://www.google.com",
        .event_handler = HttpEventHandler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No internet connection");
        esp_http_client_cleanup(client);
        return false;
    }
    ESP_LOGI(TAG, "Connected to the internet");
    esp_http_client_cleanup(client);
    return true;
}
