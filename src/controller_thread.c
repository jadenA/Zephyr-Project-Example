/*
* FILENAME :        controller_thread.c
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
#include "controller_thread.h"

// Other Includes
#include "globals.h"
#include "rtos_hal.h"
#include "core_general.h"
#include "message_setup_hal.h"
#include "controller_hal.h"


// Global Thread Variables - (Stored in rtos_hal.c)
extern k_tid_t tid_controller_thread;
extern struct k_thread controller_thread_id;
extern k_thread_stack_t controller_thread_stack;


// Setup the controller thread within Zephyr
void controller_thread_setup(void)
{
	// Start the Controller thread
	tid_controller_thread = k_thread_create(&controller_thread_id, &controller_thread_stack, CONTROLLER_THREAD_STACKSIZE, controller_thread, NULL, NULL, NULL, CONTROLLER_THREAD_PRIORITY, 0, K_NO_WAIT);
	
	k_thread_name_set(tid_controller_thread, "Controller Thread"); // Used for debug when using "Thread Analyzer"

	return;
}

// Shutdown the controller thread within Zephyr
void controller_thread_shutdown(void)
{
	// Kill the running Controller thread 
	k_thread_abort(&controller_thread_id);

	return;
}

// Setup the function the controller thread runs
void controller_thread(void) 
{

	// Do the FSM Loop 1 - Standard Logger FSM
	controller_hal_fsm_1();

	return;
}