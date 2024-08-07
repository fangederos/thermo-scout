#include <sensors/sensors.h>  // sensors library for hardware monitoring
#include "logger.h"       // Header file for logging functions
#include "sensor.h"       // Header file for sensor reading functions

#include <stdio.h>        // Standard I/O library
#include <ncurses.h>      // ncurses library for terminal handling
#include <unistd.h>       // POSIX operating system API
#include <stdlib.h>       // Standard library for memory allocation and conversion functions

#define DEFAULT_INTERVAL 1  // Default interval for updating readings in seconds

int main(int argc, char *argv[]) {
    float interval = DEFAULT_INTERVAL;  // Initialize interval with default value

    // Check if an interval is provided as a command-line argument
    if (argc > 1) {
        interval = atof(argv[1]);  // Convert the argument to an integer
        if (interval <= 0 || interval > 1) {  // Validate the interval value
            fprintf(stderr, "Invalid interval specified. Using default interval of %d second(s).\n", DEFAULT_INTERVAL);
            interval = DEFAULT_INTERVAL;  // Revert to default interval if invalid
        }
    }

    // Initialize ncurses mode
    initscr();          // Initialize the ncurses screen
    timeout(100);      // Set input timeout for getch()
    noecho();           // Disable echoing of typed characters
    cbreak();           // Disable line buffering, making input immediately available
    WINDOW *win = newwin(24, 80, 0, 0);  // Create a new window with dimensions 24x80 starting at position (0, 0)

    // Initialize sensors library
    sensors_init(NULL);  // Initialize the sensors library

    // Main loop to continuously read and log sensor data
    while (1) {
        wclear(win);  // Clear the window content

        // Function to read sensor data and log it, while also printing to the window
        read_and_log_sensors(win);  

        wrefresh(win);  // Refresh the window to show the updated content
        usleep(interval * 1000000);  // Usleep for the specified interval before the next update (w/ fractional support)
		
		if (getch() == 'q') {
			break;
		}
    }

    // Cleanup resources before exiting the program
    sensors_cleanup();  // Cleanup sensors library resources
    close_logger();     // Close the logger
    endwin();           // End ncurses mode

    return 0;  // Return success status
}
