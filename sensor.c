#include "sensor.h"    // Header file for this module
#include "logger.h"    // Header file for logging functions
#include <sensors/sensors.h>  // sensors library for hardware monitoring

#include <stdio.h>     // Standard I/O library
#include <unistd.h>    // POSIX operating system API
#include <ncurses.h>   // ncurses library for terminal handling
#include <time.h>      // Time handling functions

// Function to read sensor data, log it, and print it to the ncurses window
void read_and_log_sensors(WINDOW *win) {
    const sensors_chip_name *chip;  // Pointer to hold the detected chip name
    int c = 0;  // Counter for detected chips

    // Get the current time and print it at the top of the window
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    wattron(win, A_BOLD);  // Enable bold text attribute
    mvwprintw(win, 0, 0, "Current Time: %s", time_str);
    wattroff(win, A_BOLD);  // Disable bold text attribute

    // Print the header for the sensor readings table
    mvwprintw(win, 1, 0, "------------------------------------------------------------");
    mvwprintw(win, 2, 0, "Device          | Sensor          | Temperature (°C)");
    mvwprintw(win, 3, 0, "------------------------------------------------------------");

    int line = 4;  // Variable to keep track of the current line in the window

    // Loop through detected chips
    while ((chip = sensors_get_detected_chips(NULL, &c)) != NULL) {
        int has_temp_sensor = 0;  // Flag to check if the chip has a temperature sensor
        const sensors_feature *feature;  // Pointer to hold the detected feature
        int f = 0;  // Counter for detected features

        // Loop through features of the detected chip to find temperature sensors
        while ((feature = sensors_get_features(chip, &f)) != NULL) {
            const sensors_subfeature *sub;  // Pointer to hold the detected subfeature
            int s = 0;  // Counter for detected subfeatures

            // Loop through subfeatures to find temperature inputs
            while ((sub = sensors_get_all_subfeatures(chip, feature, &s)) != NULL) {
                if (sub->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                    has_temp_sensor = 1;  // Set the flag if a temperature sensor is found
                    break;  // Exit the subfeature loop
                }
            }
            if (has_temp_sensor) break;  // Exit the feature loop if a temperature sensor is found
        }

        if (!has_temp_sensor) continue;  // Skip to the next chip if no temperature sensor is found

        // Print the chip name as the device header
        wattron(win, A_BOLD);  // Enable bold text attribute
        mvwprintw(win, line++, 0, "Device: %s", chip->prefix);
        wattroff(win, A_BOLD);  // Disable bold text attribute

        f = 0;  // Reset the feature counter
        // Loop through features again to print the sensor readings
        while ((feature = sensors_get_features(chip, &f)) != NULL) {
            int s = 0;  // Reset the subfeature counter
            const sensors_subfeature *sub;

            // Loop through subfeatures to get temperature readings
            while ((sub = sensors_get_all_subfeatures(chip, feature, &s)) != NULL) {
                if (sub->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                    double temp;  // Variable to hold the temperature reading
                    // Get the temperature value
                    if (sensors_get_value(chip, sub->number, &temp) == 0) {
                        char value[16];  // Buffer to hold the formatted temperature value
                        snprintf(value, sizeof(value), "%.2f", temp);  // Format the temperature value
                        write_log(LOG_INFO, chip->prefix, feature->name, value);  // Log the sensor reading
                        // Print the sensor reading to the window
                        mvwprintw(win, line++, 0, "%-15s | %-15s | %.2f", chip->prefix, feature->name, temp);
                    }
                }
            }
        }

        // Print a separator line after each device's sensors
        mvwprintw(win, line++, 0, "------------------------------------------------------------");
    }
}
