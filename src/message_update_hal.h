#ifndef MESSAGE_UPDATE_HAL_H
#define MESSAGE_UPDATE_HAL_H

// includes
#include "cloud_hal.h"

// Setup the message structure for sensor data going to AWS
void message_update_hal_aws(struct cloud_message_type*, int);

// Setup the message structure for sensor data going to Lora
void message_update_hal_lora(struct cloud_message_type*, int);

// Get the Timestamp Struct from the ROOT JSON object
struct timestamp_message_type message_update_hal_timestamp_struct_from_root_json(cJSON *);

// This function updates the unix timestamp of a timestamp message
struct timestamp_message_type message_update_hal_timestamp_update_unix(struct timestamp_message_type *);

#endif