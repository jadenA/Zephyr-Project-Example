#ifndef GLOBAL_H
#define GLOBAL_H

#define APP_NAME "RyparLogger"

#define AWS_PUB_TOPIC_NAME "Data" // NOTE: AWS will need to be updated if changed (And persistance session cleared)
#define AWS_PUB_ACK_TOPIC_NAME "Data/ACK" // NOTE: AWS will need to be updated if changed (And persistance session cleared)

#define AWS_SUB_TOPIC_NAME "Config" // NOTE: AWS will need to be updated if changed (And persistance session cleared)
#define AWS_SUB_ACK_TOPIC_NAME "Config/ACK" // NOTE: AWS will need to be updated if changed (And persistance session cleared)

// Application Firmware Version
#define FW_VER_MAJOR "2"
#define FW_VER_MINOR "0"
#define FW_VER_BUILD "73"

// AWS API Version
#define AWS_API_VER_MAJOR "1" 
#define AWS_API_VER_MINOR "0"

// Types of Cores - NOTE: This should be a Kconfig in the future for easy building
#define CORE_LTE 1
#define CORE_LORA 0
#define CORE_BLE 0
#define CORE_WIFI 0

// Types of Connectivity - NOTE: This should be a Kconfig in the future for easy building
#define CONN_LTE_AWS 1
#define CONN_WIFI_AWS 0
#define CONN_LORA_AWS 0
#define CONN_SAT_SWARM 0

// AWS Settings
#define AWS_CONNECTION_RETRIES_BEFORE_RESTART 4 // Number of AWS Connection timeouts before rebooting the system
#define AWS_INIT_CONNECT_TIMEOUT 60
#define AWS_THREAD_TIMEOUT 120
#define AWS_LTE_CONNECT_TIMEOUT 30
#define AWS_INITIAL_CONNECT_TIMEOUT 30 // Initial timeout for the AWS Connect to respond with "AWS Ready"
#define AWS_RECONNECT_TIMEOUT 10 // Re-connect timeout for the AWS Connect to respond with "AWS Ready" after already connecting sucessfully 
#define AWS_TIME_UPDATE_TIMOUT 10 // Time to wait for a valid time to be obtained after sending messages and before lte de-init
#define AWS_FOTA_DOWNLOAD_TIMOUT 500 // Time to wait for a FOTA download to complete

// Other Useful Stuff
#define UNIX_TIME_2020 1577800801000 // UNIX Time for year 2020
#define CSV_MAX_FILE_LENGTH 10000000 // Max number of Bytes for the CSV file ~10MB
#define SYSTEM_GLOBAL_WATCHDOG_TIMEOUT 90000 //(25 Hours)  //44100 //(12 Hours, 15 Min) //3600 //(1 Hour) // Time in Seconds that the Watchdog with wait between "pets" before rebooting the system

// SD Card Setting
#define SD_CARD_MAX_UTIL 80 // Equated to max % full before data is purged from the SD card



#if defined(CONFIG_S1_MK2)

// Voltage Levels For Active And Throttled Modes
#define STARTUP_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the startup sleep state
#define INTERVAL_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the interval sleep state
#define INTERVAL_SLEEP_THROTTLED_BATTERY_VOLTAGE 3200 // Minimum battery voltage to get out of the throttled state
#define INTERVAL_SENSOR_CONSTANT_POWER_MIN_BATTERY_VOLTAGE 3200 // Minimum battery voltage to keep the sensor constantly powered // ONLY MK2

#endif //CONFIG_S1_MK2

#if defined(CONFIG_S1_Mini)


// Voltage Levels For Active And Throttled Modes
#define STARTUP_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the startup sleep state
#define INTERVAL_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the interval sleep state
#define INTERVAL_SLEEP_THROTTLED_BATTERY_VOLTAGE 3300 // Minimum battery voltage to get out of the throttled state
#define INTERVAL_SENSOR_CONSTANT_POWER_MIN_BATTERY_VOLTAGE 3200 // Minimum battery voltage to keep the sensor constantly powered // ONLY MK2

#endif //CONFIG_S1_Mini

#if defined(CONFIG_S1_MK1)

// Voltage Levels For Active And Throttled Modes
#define STARTUP_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the startup sleep state
#define INTERVAL_SLEEP_MIN_BATTERY_VOLTAGE 3000 // Minimum battery voltage to get out of the interval sleep state
#define INTERVAL_SLEEP_THROTTLED_BATTERY_VOLTAGE 3300 // Minimum battery voltage to get out of the throttled state
#define INTERVAL_SENSOR_CONSTANT_POWER_MIN_BATTERY_VOLTAGE 3200 // Minimum battery voltage to keep the sensor constantly powered // ONLY MK2

#endif //CONFIG_S1_MK1

// Default Flash Config Values
// --- SYSTEM FLASH DEFAULT SETTINGS ---
#define DEEP_SLEEP_STATE_DEFAULT_VALUE 0
#define SYSTEM_LOG_INTERVAL_DEFAULT_VALUE 15
#define SYSTEM_SEND_INTERVAL_DEFAULT_VALUE 4
#define SYSTEM_GPS_INTERVAL_DEFAULT_VALUE 16
#define SYSTEM_GPS_TIMEOUT_DEFAULT_VALUE 150
#define SYSTEM_VOLTAGE_ACTIVE_LOCKOUT_DEFAULT_VALUE 3000
#define SYSTEM_VOLTAGE_STARTUP_LOCKOUT_DEFAULT_VALUE 3200
#define SYSTEM_VOLTAGE_LEVEL_1_DEFAULT_VALUE 3400
#define SYSTEM_VOLTAGE_LEVEL_2_DEFAULT_VALUE 3300
#define SYSTEM_VOLTAGE_LEVEL_3_DEFAULT_VALUE 3200
#define SYSTEM_VOLTAGE_LEVEL_4_DEFAULT_VALUE 3100
#define SYSTEM_SENSOR_CONSTANT_POWER_VALUE 0
#define SYSTEM_COMMS_CONSTANT_POWER_VALUE 0
#define SYSTEM_ENABLE_READING_TRIGGER_DEFAULT_VALUE 0
#define SYSTEM_GPS_FORCE_READ_DEFAULT_VALUE 0
#define SYSTEM_FLASH_UPDATED_FLAG_DEFAULT_VALUE 0
#define SYSTEM_FORCE_PRINT_DEFAULT_VALUE 0

// --- SENSOR FLASH DEFAULT SETTINGS ---
#define SENSOR_1_ENABLED_DEFAULT_VALUE 0
#define SENSOR_1_TYPE_DEFAULT_VALUE 0
#define SENSOR_1_ID_DEFAULT_VALUE 0
#define SENSOR_1_TIMEZONE_DEFAULT_VALUE 600

#define SENSOR_2_ENABLED_DEFAULT_VALUE 0
#define SENSOR_2_TYPE_DEFAULT_VALUE 0
#define SENSOR_2_ID_DEFAULT_VALUE 0
#define SENSOR_2_TIMEZONE_DEFAULT_VALUE 600

#define SENSOR_3_ENABLED_DEFAULT_VALUE 0
#define SENSOR_3_TYPE_DEFAULT_VALUE 0
#define SENSOR_3_ID_DEFAULT_VALUE 0
#define SENSOR_3_TIMEZONE_DEFAULT_VALUE 600

#define SENSOR_4_ENABLED_DEFAULT_VALUE 0
#define SENSOR_4_TYPE_DEFAULT_VALUE 0
#define SENSOR_4_ID_DEFAULT_VALUE 0
#define SENSOR_4_TIMEZONE_DEFAULT_VALUE 600

// --- PULSE VALUE ---
#define PULSE_1_COUNT_DEFAULT_VALUE 0
#define PULSE_2_COUNT_DEFAULT_VALUE 0
#define UNIX_TIME_BACKUP_DEFAULT_VALUE 1704695165374

// --- CLOUD FLASH DEFAULT SETTINGS ---
#define AWS_URL_DEFAULT_LENGTH 100
#define AWS_CERT_ID_DEFAULT_VALUE 291836
#define AWS_CONNECTION_TIMEOUT_DEFAULT_VALUE 60
#define AWS_SEND_TIMEOUT_DEFAULT_VALUE 120
#define AWS_FOTA_TIMEOUT_DEFAULT_VALUE 300
#define AWS_RETRIES_DEFAULT_VALUE 2

// --- LTE FLASH DEFAULT SETTINGS ---
#define LTE_APN_DEFAULT_VALUE "\0"
#define LTE_APN_DEFAULT_LENGTH 100
#define LTE_BAND_1_DEFAULT_VALUE 0
#define LTE_BAND_2_DEFAULT_VALUE 0
#define LTE_BAND_3_DEFAULT_VALUE 0
#define LTE_BAND_4_DEFAULT_VALUE 0
#define LTE_BAND_MAX_SIZE 88

// Sensor Types
#define SENSOR_TYPE_NONE 0
#define SENSOR_TYPE_RS485 1
#define SENSOR_TYPE_RS232 2
#define SENSOR_TYPE_SDI12 3
#define SENSOR_TYPE_PULSE 4
#define SENSOR_TYPE_4_20MA 5

#define SENSOR_ENABLED 1
#define SENSOR_DISABLED 0


#endif