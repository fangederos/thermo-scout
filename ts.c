#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>     // Include this header for atoi
#include <sensors/sensors.h>
#include "logger.h"
#include "sensor.h"

#define DEFAULT_INTERVAL 1  // Update every second

int main(int argc, char *argv[]) {
    int interval = DEFAULT_INTERVAL;

    if (argc > 1) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            fprintf(stderr, "Invalid interval specified. Using default interval of %d second(s).\n", DEFAULT_INTERVAL);
            interval = DEFAULT_INTERVAL;
        }
    }

    initscr();           // Start curses mode
    timeout(5000);       // Set timeout for getch()
    noecho();            // Do not echo input characters
    cbreak();            // Disable line buffering
    WINDOW *win = newwin(24, 80, 0, 0);

    sensors_init(NULL);  // Initialize sensors library

    while (1) {
        wclear(win);
        
        read_and_log_sensors(win);  // Pass the ncurses window to the function

        wrefresh(win);  // Refresh the window to show the updates
        sleep(interval); // Sleep for the specified interval
    }

    sensors_cleanup();
    close_logger();
    endwin();  // End curses mode

    return 0;
}
