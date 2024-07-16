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
    wattron(win, A_BOLD | COLOR_PAIR(1));
    wprintw(win, "Current Time: %s\n\n", time_str);
    wattroff(win, A_BOLD | COLOR_PAIR(1));
}

void print_header(WINDOW *win) {
    wattron(win, A_REVERSE);
    wprintw(win, "%-20s%-20s%-20s\n", "Device", "Sensor", "Temperature (°C)");
    wattroff(win, A_REVERSE);
    wprintw(win, "------------------------------------------------------------\n");
}

void print_device_header(WINDOW *win, const char *device) {
    wattron(win, A_BOLD);
    wprintw(win, "\nDevice: %s\n", device);
    wattroff(win, A_BOLD);
}

void print_sensor_reading(WINDOW *win, const char *device, const char *sensor, double temp) {
    wprintw(win, "  %-20s%-20s%-20.2f\n", device, sensor, temp);
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
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    cbreak();
    noecho();
    curs_set(FALSE);

    WINDOW *win = newwin(0, 0, 0, 0);

    while (1) {
        werase(win);  // Clear the window
        print_current_time(win);  // Print current local time
        print_header(win);  // Print the table header

        const sensors_chip_name *chip;
        int c = 0;
        while ((chip = sensors_get_detected_chips(NULL, &c)) != NULL) {
            int has_temp_sensor = 0;
            const sensors_feature *feature;
            int f = 0;
            while ((feature = sensors_get_features(chip, &f)) != NULL) {
                const sensors_subfeature *sub;
                int s = 0;
                while ((sub = sensors_get_all_subfeatures(chip, feature, &s)) != NULL) {
                    if (sub->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                        has_temp_sensor = 1;
                        break;
                    }
                }
                if (has_temp_sensor) break;
            }
            if (!has_temp_sensor) continue;

            print_device_header(win, chip->prefix);  // Print device header
            f = 0;
            while ((feature = sensors_get_features(chip, &f)) != NULL) {
                int s = 0;
                const sensors_subfeature *sub;
                while ((sub = sensors_get_all_subfeatures(chip, feature, &s)) != NULL) {
                    if (sub->type == SENSORS_SUBFEATURE_TEMP_INPUT) {
                        double temp;
                        if (sensors_get_value(chip, sub->number, &temp) == 0) {
                            char value[16];
                            snprintf(value, sizeof(value), "%.2f", temp);
                            write_log(LOG_INFO, chip->prefix, feature->name, value);
                            print_sensor_reading(win, chip->prefix, feature->name, temp);
                        }
                    }
                }
            }
            wprintw(win, "------------------------------------------------------------\n");
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
