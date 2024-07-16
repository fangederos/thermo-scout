#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sensors/sensors.h>
#include <curses.h>
#include "logger.h"

void print_current_time(WINDOW *win) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    wprintw(win, "Current Time: %s\n", time_str);
}

int main(int argc, char *argv[]) {
    int interval = 1; // Default interval
    if (argc > 1) {
        interval = atoi(argv[1]);
    }

    init_logger("sensor.log");

    if (sensors_init(NULL)) {
        fprintf(stderr, "sensors_init failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize curses
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);

    WINDOW *win = newwin(0, 0, 0, 0);

    while (1) {
        werase(win);  // Clear the window
        print_current_time(win);  // Print current local time

        const sensors_chip_name *chip;
        int c = 0;
        while ((chip = sensors_get_detected_chips(NULL, &c)) != NULL) {
            wprintw(win, "Device: %s\n", chip->prefix);
            const sensors_feature *feature;
            int f = 0;
            while ((feature = sensors_get_features(chip, &f)) != NULL) {
                const sensors_subfeature *sub;
                int s = 0;
                while ((sub = sensors_get_all_subfeatures(chip, feature, &s)) != NULL) {
                    if (sub->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                        double temp;
                        if (sensors_get_value(chip, sub->number, &temp) == 0) {
                            char value[16];
                            snprintf(value, sizeof(value), "%.2f", temp);
                            write_log(LOG_INFO, chip->prefix, feature->name, value);
                            wprintw(win, "  %s-%s: %.2f°C\n", chip->prefix, feature->name, temp);
                        }
                    }
                }
            }
            wprintw(win, "------------------------\n");
        }
        wrefresh(win);  // Refresh the window to show the updates
        sleep(interval);
    }

    // Cleanup
    sensors_cleanup();
    close_logger();
    endwin();  // End curses mode

    return 0;
}
