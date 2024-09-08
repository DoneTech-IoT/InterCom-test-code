#ifndef _JSON_UTILITY_H_
#define _JSON_UTILITY_H_
/* 
 * SEZ@Done (ehsan.ziyaee@gmail.com)
 * init 2024.01.29
 * some method for debugging json.
 */

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include <stdarg.h>
#include "cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/**
 * @brief This function searches for a specific value in a JSON string based on a given key.
 * @param[in] jsonString The JSON string to search within.
 * @param[in] key The key to search for.
 * @param[out] valueBuffer The buffer to store the found value.
 * @param[in] bufferSize The size of the value buffer.
 * @return Returns true if the value is found and copied to the value buffer, and false otherwise.
 */
char *FindValueByKey(const char *jsonStr, const char *key, char *ValueBuffer, size_t SizeOfValueBuf);

/**
 * @brief This function counts the number of key-value pairs in a JSON string.
 * @param[in] jsonString The JSON string to count the key-value pairs.
 * @return The number of key-value pairs.
 */
int CountOfKeyValues(const char *jsonStr);

/**
 * @brief This function parses a JSON string and prints the key-value pairs.
 * @param[in] jsonString The JSON string to parse and print.
 */
void ParseOutputJSON(const char *jsonStr);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //_JSON_UTILITY_H_