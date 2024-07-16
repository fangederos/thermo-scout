#include "sensor.h"
#include "logger.h"
#include <sensors/sensors.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

void read_and_log_sensors(WINDOW *win) {
    const sensors_chip_name *chip;
    int c = 0;

    // Print current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 0, "Current Time: %s", time_str);
    wattroff(win, A_BOLD);

    mvwprintw(win, 1, 0, "------------------------------------------------------------");
    mvwprintw(win, 2, 0, "Device          | Sensor          | Temperature (°C)");
    mvwprintw(win, 3, 0, "------------------------------------------------------------");

    int line = 4; // Start printing sensor details from this line

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

        // Print device header
        wattron(win, A_BOLD);
        mvwprintw(win, line++, 0, "Device: %s", chip->prefix);
        wattroff(win, A_BOLD);

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
                        mvwprintw(win, line++, 0, "%-15s | %-15s | %.2f", chip->prefix, feature->name, temp);
                    }
                }
            }
        }

        mvwprintw(win, line++, 0, "------------------------------------------------------------");
    }
}
