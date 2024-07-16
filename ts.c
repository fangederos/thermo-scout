#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sensors/sensors.h>
#include "logger.h"

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

    while (1) {
        const sensors_chip_name *chip;
        int c = 0;
        while ((chip = sensors_get_detected_chips(NULL, &c)) != NULL) {
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
                            printf("%s-%s: %.2f\n", chip->prefix, feature->name, temp);
                        }
                    }
                }
            }
        }
        sleep(interval);
    }

    sensors_cleanup();
    close_logger();
    return 0;
}
