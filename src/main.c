/*
* FILENAME :        main.c
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

// Global Include
#include "globals.h"

// Thread Includes
#include "watchdog_thread.h"
#include "storage_thread.h"
#include "cli_thread.h"
#include "led_thread.h"
#include "button_thread.h"
#include "controller_thread.h"
#include "sensor_thread.h"
#include "gnss_thread.h"
#include "aws_thread.h"

// Other Includes
#include "core_lte.h"
#include "cjson_hal.h"
#include "i2c_hal.h"
#include "aws_hal.h"
#include "aws_handler.h"
#include "button_hal.h"
#include "config_hal.h"
#include "pulse_hal.h"
#include "led_hal.h"
#include "ltc4162_hal.h"
#include "mb85rc64ta_hal.h"



void main(void) {

#if defined(CONFIG_S1_MK1)
	printk("\n");
	printk("Board = S1 Mk1\n");
	printk("Rypar Logger Version: %s.%s.%s\n", FW_VER_MAJOR, FW_VER_MINOR, FW_VER_BUILD);
	printk("\n");
#endif //CONFIG_S1_MK1

#if defined(CONFIG_S1_MK2)
	printk("\n");
	printk("Board = S1 Mk2\n");
	printk("Rypar Logger Version: %s.%s.%s\n", FW_VER_MAJOR, FW_VER_MINOR, FW_VER_BUILD);
	printk("\n");
#endif //CONFIG_S1_MK2

#if defined(CONFIG_S1_Mini)
	printk("\n");
	printk("Board = S1 Mini\n");
	printk("Rypar Logger Version: %s.%s.%s\n", FW_VER_MAJOR, FW_VER_MINOR, FW_VER_BUILD);
	printk("\n");
#endif //CONFIG_S1_Mini

	// Init the required periferals
	core_general_periferal_init();
	
	// Put nessasary device configuration here prior to threads starting
	rtos_hal_init(); // Setup RTOS 
	json_hal_init(); // Setup JSON library
	config_hal_init(); // Pull out the flash config
	i2c_hal_init(); // Init I2C 
	led_hal_init(); // Init LED's

	// Init the Pulse Sensor IO
	pulse_hal_init();

	// Init the user button
	button_hal_init();

// Config the MPPT Charger on the Mk2 - (LTC4162)
#if defined(CONFIG_S1_MK2)

	// Setup the MPPT Charger on the MK2
	ltc4162_hal_init();

	// Init the SD Card
	sd_logging_hal_setup();

#endif //CONFIG_S1_MK2

#if defined(CONFIG_S1_Mini)

	// Init the SD Card
	sd_logging_hal_setup();

#endif //CONFIG_S1_Mini

	// Pull the Flash Value for Deep Sleep
	uint8_t deepSleepState = 0;
	config_hal_read_deep_sleep_state(&deepSleepState);

	// If the deep sleep state is set then go into deep sleep and  wait for a button press event
	if(deepSleepState == 1) {

		printk("INFO: DEEP SLEEP STATE ENABLED, GOING TO DEEP SLEEP, BUTTON PRESS REQUIRED TO WAKE UP\n");

		// Wait for the USR Button to be heild for X seconds 
		button_hal_deep_sleep_wakeup();

		printk("INFO: Deep Sleep Ending, Normal FSM Starting\n");

	// If we don't go into deep sleep state, Do a startup Sleep 
	} else {

		printk("INFO: Deep Sleep NOT enabled, Going into startup sleep\n");

		// Shut down Periferals and go into startup sleep
		core_general_powerup_sleep(); // NOTE: If battery voltage is too low this will not yeild

		led_hal_do_startup_lights(5);
	}

	// --- Start The Threads ---

	watchdog_thread_setup();

	// Start the Controller Thread
	controller_thread_setup();

	// Start the Button Thread
	button_thread_setup();

	core_lte_set_modem_apn(); 
	core_lte_set_modem_bands(); 

#if defined(CONFIG_S1_MK2)

	// Setup the CLI Thread
	cli_thread_setup();

#endif //CONFIG_S1_MK2

#if defined(CONFIG_S1_Mini)

	// Setup the CLI Thread
	cli_thread_setup();

#endif //CONFIG_S1_Mini
	
}
