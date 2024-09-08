#ifndef DONE_MATTER_ENDPOINT_H
#define DONE_MATTER_ENDPOINT_H

#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_identify.h>

//SEZ@Done: we define custom ID 
#define ESP_MATTER_DONE_TEST_DEVICE_TYPE_ID 0x1000
#define ESP_MATTER_DONE_TEST_DEVICE_TYPE_VERSION 1

#define ESP_MATTER_DONE_MASTER_POWER_KEY_DEVICE_TYPE_ID 0x1001
#define ESP_MATTER_DONE_MASTER_POWER_KEY_DEVICE_TYPE_VERSION 1

#define ESP_MATTER_DONE_MULTI_FUNCTION_SWITCH_DEVICE_TYPE_ID 0x1002
#define ESP_MATTER_DONE_MULTI_FUNCTION_SWITCH_DEVICE_TYPE_VERSION 1

namespace esp_matter {
namespace endpoint {

namespace DoneMasterPowerKey {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;   
    cluster::boolean_state::config_t boolean;
    cluster::on_off::config_t on_off;    
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding;         
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_MasterPowerKey

namespace DoneMultiFunctionSwitch {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;    
    cluster::on_off::config_t on_off;    
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding;         
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_MultiFunctionSwitch

} /* endpoint */
} /* esp_matter */

#endif //DONE_MATTER_ENDPOINT_H