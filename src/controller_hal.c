/*
* FILENAME :        controller_hal.c
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

// Header File
#include "controller_hal.h"

// Other Includes
#include "globals.h"
#include "rtos_hal.h"
#include "controller_thread.h"
#include "core_general.h"
#include "message_setup_hal.h"
#include "sat_iridium_hal.h"




// This is the standard FSM Loop for the Logger Application
void controller_hal_fsm_1(void)
{

	// Variable to store initial loop
	int startupLoop = 1;

	// Setup Variables to Store the Config Values
	uint16_t systemSendInterval = 0;
	uint16_t systemGPSInterval = 0;
	uint16_t systemGPSTimeout = 0;
	int systemGPSReadInterval = 0;

	// Clear all the message queues (System, GNSS, Timestamp and 4 x Sensor)
	message_setup_hal_clear_temp_message_queues();

	while(1){

		// Get the config values from Flash
		config_hal_read_system_send_interval(&systemSendInterval);
		config_hal_read_system_gps_interval(&systemGPSInterval);
		config_hal_read_system_gps_timeout(&systemGPSTimeout);

		// Calculate the GPS interval (Example: Send = 4, GPS = 2, GPS Read Int = 8)
		// Also check if GPS is enabled to ensure we don't div by 0
		if(systemGPSInterval != 0) {
			systemGPSReadInterval = systemSendInterval * systemGPSInterval;
		} else {
			systemGPSReadInterval = systemSendInterval;
		}

		// Itterate through the multiple of Send and GPS intervals
		for (int i = 1; i <= systemGPSReadInterval; ++i) {
			
			// Check if GPS is enabled to ensure we don't div by 0
			if(systemGPSInterval != 0) {
				printk("\n");
				printk("Message Loop = %d : Send = %d : GPS Read = %d : GPS Send %d\n", i, (i % systemSendInterval == 0), ((i + 1) % systemGPSReadInterval == 0), (i % systemGPSReadInterval == 0));
			} else {
				printk("\n");
				printk("Message Loop = %d : Send = %d : GPS = Disabled\n", i, (i % systemSendInterval == 0));
				
			}
			
			// Print out all the info about the threads
			//rtos_hal_print_thread_info();

			// Print out the heap info
			rtos_hal_print_heap_info();

			// Setup Data Collection Threads (Gather all the data for the message thread)
			system_data_thread_setup();
			timestamp_thread_setup();

			// Wait for the Data Collection Threads to finish (With Reasonable Timeouts)
			
			system_data_thread_wait_till_complete(20);
			timestamp_thread_wait_till_complete(10);

			// Read from the attached sensor and wait for it to complete
			sensor_thread_setup();
			sensor_thread_wait_till_complete(100);

			// Setup the message thread and wait for it to finish
			message_thread_setup();
			message_thread_wait_till_complete(30);

			// Clear all the message queues (System, GNSS, Timestamp and 4 x Sensor)
			message_setup_hal_clear_temp_message_queues();

			// Check if this is the upload interval - Or if it is the first loop
			if(((i % systemSendInterval == 0)) || (startupLoop == 1)) {

				cloud_thread_setup();
				cloud_thread_wait_till_complete(120 + AWS_FOTA_DOWNLOAD_TIMOUT);
			}

			// Check if the next interval is the GPS Read interval
			// Also check if GPS is enabled to ensure we don't div by 0
			if((systemGPSInterval != 0) && ((i + 1) % systemGPSReadInterval) == 0) {

				gnss_thread_setup();
				gnss_thread_wait_till_complete(systemGPSTimeout);
			}

			// Go to sleep once the message thread has finished
			core_general_interval_sleep(); // NOTE: If battery voltage is too low this will not yeild

			// Set to 0 to show that the startup loop has completed
			startupLoop = 0;
		}
	}

	return;
}
