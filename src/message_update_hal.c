/*
* FILENAME :        message_update_hal.c
*
*       Copyright Rypar Pty Ltd 2023.  All rights reserved.
*
* AUTHOR :    Jaden Arndt        START DATE :    14/03/2023
*
* CHANGES :
*
*/

// Standard Zephyr Includes
#include <zephyr/kernel.h>
#include <stdio.h>
#include <stdlib.h>

// Zephyr Optional Includes
#include "cjson_hal.h"
#include <string.h>

// Header File
#include "message_update_hal.h"

// Other Includes
#include "message_setup_hal.h"
#include "cloud_hal.h"
#include "globals.h"


// Setup the message structure for sensor data going to AWS
void message_update_hal_aws(struct cloud_message_type* message_data, int rsrpValue) {

	// Variables required
	int err = 0;

	// Create an Empty Struct
	struct timestamp_message_type timestamp_data;

	// Create the JSON objects
	cJSON *root_obj = NULL;
	cJSON *timestamp_obj_new = NULL;

	// Parse the JSON info
	root_obj = cJSON_Parse(message_data->message);
	if (root_obj == NULL) {
		printk("ERROR: cJSON Parse Root Object failure\n");
		return;
	}

	// Pull the timestamp struct data out of the Root JSON object
	timestamp_data = message_update_hal_timestamp_struct_from_root_json(root_obj);

	// Send off the timestamp data to have the unix time updated
	struct timestamp_message_type timestamp_unix_updated = message_update_hal_timestamp_update_unix(&timestamp_data);

	timestamp_obj_new = cJSON_CreateObject();

	// Check if the new object was created ok
	if(timestamp_obj_new != NULL) {

		int error = message_setup_hal_timestamp_struct_to_json(timestamp_obj_new, &timestamp_unix_updated);

		if(error == 0) {

			// Remove the existing timestamp object
			cJSON_DeleteItemFromObject(root_obj, "time");

			// Add the new timestamp object
			err += json_add_obj(root_obj, "time", timestamp_obj_new);
			
		} else {
			printk("ERROR: Not able to convert Timestamp Struct to JSON\n");
			cJSON_Delete(timestamp_obj_new);
		}

	} else {
		cJSON_Delete(timestamp_obj_new);
	}

	// Add the rsrp to the root object
	err += json_add_number(root_obj, "rsrp", rsrpValue); 

	// Check if everything has been added without error
	if (err) {
		printk("ERROR: Not Able To Add RSRP json_add, error: %d\n", err);
		goto cleanup;
	}

	// Print the JSON object to text
	message_data->messageAWS = cJSON_Print(root_obj);
	if (message_data->messageAWS == NULL) {
		printk("ERROR: cJSON_Print, error: returned NULL\n");
		err = -ENOMEM;
		message_data->messageAWS = NULL;
		goto cleanup;
	}	

	// Update the AWS message length
	message_data->messageAWSLength = strlen(message_data->messageAWS);

cleanup:

	// Clean up the cJSON left over data
	cJSON_Delete(root_obj);

	return;
}

// Setup the message structure for sensor data going to Lora
void message_update_hal_lora(struct cloud_message_type* message_data, int rsrpValue) {

	// Variables required
	int err = 0;

	// Create the JSON objects
	cJSON *root_obj = NULL;

	// Parse the JSON info
	root_obj = cJSON_Parse(message_data->message);
	if (root_obj == NULL) {
		printk("ERROR: cJSON Parse Root Object failure\n");
		return;
	}

	// Add the rsrp to the root object
	err += json_add_number(root_obj, "rsrp", rsrpValue); 

	// Check if everything has been added without error
	if (err) {
		printk("ERROR: Not Able To Add RSRP json_add, error: %d\n", err);
		goto cleanup;
	}

	// Print the JSON object to text
	message_data->messageAWS = cJSON_Print(root_obj);
	if (message_data->messageAWS == NULL) {
		printk("ERROR: cJSON_Print, error: returned NULL\n");
		err = -ENOMEM;
		message_data->messageAWS = NULL;
		goto cleanup;
	}	

	// Update the AWS message length
	message_data->messageAWSLength = strlen(message_data->messageAWS);

cleanup:

	// Clean up the cJSON left over data
	cJSON_Delete(root_obj);

	return;
}


// Get the Timestamp Struct from the ROOT JSON object
struct timestamp_message_type message_update_hal_timestamp_struct_from_root_json(cJSON *root_obj) {

	// Setup the Timestamp Struct
	struct timestamp_message_type timestamp_data;

	// Setup the JSON objects required
	cJSON *timestamp_obj = NULL;
	cJSON *timestamp_reading_id_obj = NULL;
	cJSON *timestamp_send_id_obj = NULL;
	cJSON *timestamp_unix_obj = NULL;
	cJSON *timestamp_ru_obj = NULL;
	cJSON *timestamp_su_obj = NULL;
	cJSON *timestamp_tc_obj = NULL;

	// Check if timestamp object is in the root object
	timestamp_obj = cJSON_GetObjectItem(root_obj, "time");

	// If no Timestamp object could be found, then fill in the data with default values
	if (timestamp_obj == NULL) {

		printk("ERROR: cJSON Parse Timestamp Object failure\n");

		// Delete the Unused Timestamp 
		cJSON_Delete(timestamp_obj);

		// Set the timestamp Caluclated Flag to Error
		timestamp_data.timeCalculatedFlag = -1;
		timestamp_data.readingSystemUptime = 0;
		timestamp_data.sendSystemUptime = 0;
		timestamp_data.readingSystemResetId = 0;
		timestamp_data.sendSystemResetId = 0;
		timestamp_data.unixTimestamp = 0;

	// If we get here the Timestamp Object was sucessfully found in the root object
	} else {

		// Pull all of the timestamp metrics out of the timestamp object
		timestamp_reading_id_obj = cJSON_GetObjectItem(timestamp_obj, "rid");
		timestamp_send_id_obj = cJSON_GetObjectItem(timestamp_obj, "sid");
		timestamp_unix_obj = cJSON_GetObjectItem(timestamp_obj, "unix");
		timestamp_ru_obj = cJSON_GetObjectItem(timestamp_obj, "ru");
		timestamp_su_obj = cJSON_GetObjectItem(timestamp_obj, "su");
		timestamp_tc_obj = cJSON_GetObjectItem(timestamp_obj, "tc");

		// Check if any of the timestamp metrics are NULL and delete them all if so
		if(timestamp_reading_id_obj == NULL || timestamp_send_id_obj == NULL || timestamp_unix_obj == NULL || timestamp_ru_obj == NULL || timestamp_su_obj == NULL || timestamp_tc_obj == NULL) {

			// Delete all the timestamp objects and error out 
			cJSON_Delete(timestamp_reading_id_obj);
			cJSON_Delete(timestamp_send_id_obj);
			cJSON_Delete(timestamp_unix_obj);
			cJSON_Delete(timestamp_ru_obj);
			cJSON_Delete(timestamp_su_obj);
			cJSON_Delete(timestamp_tc_obj);

			// Setup the timestamp struct to default values
			timestamp_data.timeCalculatedFlag = -1;
			timestamp_data.readingSystemUptime = 0;
			timestamp_data.sendSystemUptime = 0;
			timestamp_data.readingSystemResetId = 0;
			timestamp_data.sendSystemResetId = 0;
			timestamp_data.unixTimestamp = 0;

		} else {

			// Get the info from the JSON objects and put it in the timestamp struct
			timestamp_data.readingSystemResetId = cJSON_GetNumberValue(timestamp_reading_id_obj);
			timestamp_data.sendSystemResetId = cJSON_GetNumberValue(timestamp_send_id_obj);
			timestamp_data.unixTimestamp = cJSON_GetNumberValue(timestamp_unix_obj);
			timestamp_data.readingSystemUptime = cJSON_GetNumberValue(timestamp_ru_obj);
			timestamp_data.sendSystemUptime = cJSON_GetNumberValue(timestamp_su_obj);
			timestamp_data.timeCalculatedFlag = cJSON_GetNumberValue(timestamp_tc_obj);
		}
	}

	return timestamp_data;
}


// This function updates the unix timestamp of a timestamp message
struct timestamp_message_type message_update_hal_timestamp_update_unix(struct timestamp_message_type *timestamp_data) {

	// Required variables
	int err = 0;
	int64_t unix_time = 0;

	// Setup a struct to pass the updated values back 
	struct timestamp_message_type timestamp_unix_updated;

	// Copy across the current values
	timestamp_unix_updated.readingSystemUptime = timestamp_data->readingSystemUptime;
	timestamp_unix_updated.sendSystemUptime = timestamp_data->sendSystemUptime;
	timestamp_unix_updated.readingSystemResetId = timestamp_data->readingSystemResetId;
	timestamp_unix_updated.sendSystemResetId = timestamp_data->sendSystemResetId;
	timestamp_unix_updated.timeCalculatedFlag = timestamp_data->timeCalculatedFlag;
	timestamp_unix_updated.unixTimestamp = timestamp_data->unixTimestamp;

	// Try to get the current UNIX time
	core_general_get_unix_time(&unix_time);

	// Check if there is any errors in getting the timestamp
	if(timestamp_unix_updated.timeCalculatedFlag == -1) {


		// If the returned value is non-zero then it is valid
		if((unix_time != 0) && (unix_time > UNIX_TIME_2020)) {

			// No timstamp could be found but UNIX time is valid
			timestamp_unix_updated.unixTimestamp = unix_time;
			timestamp_unix_updated.sendSystemUptime = k_uptime_get();
			timestamp_unix_updated.readingSystemResetId = 0;
			timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
			timestamp_unix_updated.readingSystemUptime = 0;
			timestamp_unix_updated.timeCalculatedFlag = 6;

		} else {

			// No timstamp could be found but UNIX time is NOT valid
			timestamp_unix_updated.unixTimestamp = 0;
			timestamp_unix_updated.sendSystemUptime = k_uptime_get();
			timestamp_unix_updated.readingSystemResetId = 0;
			timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
			timestamp_unix_updated.readingSystemUptime = 0;
			timestamp_unix_updated.timeCalculatedFlag = 7;
			
		}

	// If we get here, we have no timestamp error code
	} else {

		// Check if we already have a UNIX time set for the packet
		if(timestamp_unix_updated.unixTimestamp == 0) {

			// Check that the packet came from the same reset cycle as now
			if(timestamp_unix_updated.readingSystemResetId == core_general_get_reset_uuid()) {

				// If the returned value is non-zero then it is valid
				if((unix_time != 0) && (unix_time > UNIX_TIME_2020)) {

					// If we get here we have got UNIX time OK
					timestamp_unix_updated.timeCalculatedFlag = 2; // Set to 2 to show this was calculated from current UNIX time and time offset
					timestamp_unix_updated.sendSystemUptime = k_uptime_get(); // Set to time now
					timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
					timestamp_unix_updated.unixTimestamp = unix_time - (timestamp_unix_updated.sendSystemUptime - timestamp_unix_updated.readingSystemUptime); // Calculate the UNIX time at point of sample 
				
				// If we get here the returned UNIX value is not valid
				} else {

					timestamp_unix_updated.sendSystemUptime = k_uptime_get(); // Set to time now
					timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
					timestamp_unix_updated.timeCalculatedFlag = 4;

				}

			// We get here if UNIX is not set and the reading occoured at another Reset Cycle
			} else {

				timestamp_unix_updated.sendSystemUptime = k_uptime_get(); // Set to time now
				timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
				timestamp_unix_updated.timeCalculatedFlag = 5;

			}
		
		// We can get here if UNIX time is already filled out
		} else {

			// Check if both the reading and current time are from the same reset cycle
			if(timestamp_unix_updated.readingSystemResetId == core_general_get_reset_uuid()) {

				// At this point the reading and current time are from the same reset cycle
				timestamp_unix_updated.sendSystemUptime = k_uptime_get(); // Set to time now
				timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
				timestamp_unix_updated.timeCalculatedFlag = 1; 
				
			// We get here if UNIX time is set and the sensor was read from another reset cycle
			} else {

				// At this point the reading and the current time are from different reset cycles
				timestamp_unix_updated.sendSystemUptime = k_uptime_get(); // Set to time now
				timestamp_unix_updated.sendSystemResetId = core_general_get_reset_uuid();
				timestamp_unix_updated.timeCalculatedFlag = 3; // Set to 3 as UNIX time is valid but Send and Read uptime is not from the same reset cycle

			}
		}
	}

	return timestamp_unix_updated;
}