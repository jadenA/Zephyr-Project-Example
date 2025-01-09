#ifndef CONTROLLER_THREAD_H
#define CONTROLLER_THREAD_H

// Setup the controller thread within Zephyr
void controller_thread_setup(void);

// Shutdown the controller thread within Zephyr
void controller_thread_shutdown(void);

// Setup the function the controller thread runs
void controller_thread(void);

#endif