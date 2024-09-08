#include <stdio.h>
#include "JsonUtility.h"

static const char *TAG = "JsonUtility";

/**
 * @brief This function searches for a specific value in a JSON string based on a given key.
 * @param[in] jsonString The JSON string to search within.
 * @param[in] key The key to search for.
 * @param[out] valueBuffer The buffer to store the found value.
 * @param[in] bufferSize The size of the value buffer.
 * @return Returns true if the value is found and copied to the value buffer, and false otherwise.
 */
char *FindValueByKey(const char *jsonStr, const char *key, char *ValueBuffer, size_t SizeOfValueBuf)
{
    cJSON *root = cJSON_Parse(jsonStr);
    if (root == NULL)
    {
        ESP_LOGE(TAG,"Failed to parse JSON.\n");
        return NULL;
    }
    cJSON *item = cJSON_GetObjectItemCaseSensitive(root, key);
    if (item == NULL)
    {
        ESP_LOGE(TAG,"Key '%s' not found.\n", key);
        cJSON_Delete(root);
        return NULL;
    }
    //  value = item->valuestring;
    strncpy(ValueBuffer, item->valuestring, sizeof(ValueBuffer) - 1);
    ValueBuffer[sizeof(ValueBuffer) - 1] = '\0';
    cJSON_Delete(root);
    return ValueBuffer;
}

/**
 * @brief This function counts the number of key-value pairs in a JSON string.
 * @param[in] jsonString The JSON string to count the key-value pairs.
 * @return The number of key-value pairs.
 */
int CountOfKeyValues(const char *jsonStr)
{
    cJSON *root = cJSON_Parse(jsonStr);
    if (root == NULL)
    {
        ESP_LOGE(TAG,"Failed to parse JSON.\n");
        return -1;
    }
    int count = cJSON_GetArraySize(root);
    ESP_LOGI(TAG, "count of keys = %d\n", count);
    cJSON_Delete(root);
    return count;
}

/**
 * @brief This function parses a JSON string and prints the key-value pairs.
 * @param[in] jsonString The JSON string to parse and print.
 */
void ParseOutputJSON(const char *jsonStr)
{
    // CountOfKeyValues(jsonStr);
    char buf[100];
    FindValueByKey(jsonStr, "Key1", buf, sizeof(buf));
    ESP_LOGI(TAG, "\n%s\n", buf);
    cJSON *root = cJSON_Parse(jsonStr);
    if (root == NULL)
    {
        ESP_LOGE(TAG,"Failed to parse JSON.\n");
        return;
    }
    cJSON *item = root->child;
    while (item != NULL)
    {
        if (cJSON_IsString(item))
        {
            ESP_LOGI(TAG, "%s: %s\n", item->string, item->valuestring);
        }
        item = item->next;
    }
    cJSON_Delete(root);
}