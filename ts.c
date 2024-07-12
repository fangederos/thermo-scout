#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "logger.h"

// Global variable to indicate if the terminal has been resized
volatile sig_atomic_t resize_flag = 0;

void read_sensors(char **buffer, size_t *size) {
    FILE *fp = popen("sensors", "r");
    if (fp == NULL) {
        snprintf(*buffer, *size, "Error: Failed to run 'sensors' command: %s", strerror(errno));
        return;
    }

    size_t capacity = *size;
    size_t length = 0;
    char temp_buffer[1024];

    while (fgets(temp_buffer, sizeof(temp_buffer), fp) != NULL) {
        size_t temp_length = strlen(temp_buffer);
        if (length + temp_length >= capacity) {
            capacity *= 2;
            *buffer = realloc(*buffer, capacity);
            if (*buffer == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                pclose(fp);
                return;
            }
        }
        strcpy(*buffer + length, temp_buffer);
        length += temp_length;
    }
    pclose(fp);
    *size = capacity;
}

void resize_handler(int signum) {
    resize_flag = 1;
}

void initialize_ncurses() {
    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    signal(SIGWINCH, resize_handler);
}

void handle_resize() {
    if (resize_flag) {
        endwin();
        refresh();
        clear();
        resize_flag = 0;
    }
}

void display_sensors_output(char *sensors_output, int height, int width) {
    clear();
    int line_count = 0;
    char *line = strtok(sensors_output, "\n");
    while (line != NULL && line_count < height - 2) {
        mvprintw(line_count + 1, 0, "%.*s", width - 1, line);
        line = strtok(NULL, "\n");
        line_count++;
    }
    refresh();
}

int has_sensor_data_changed(const char *new_data, const char *old_data) {
    return strcmp(new_data, old_data) != 0;
}

int main(int argc, char *argv[]) {

    int update_interval = 1;

    if (argc > 1) {
        update_interval = atoi(argv[1]);
        if (update_interval <= 0) {
            fprintf(stderr, "Invalid update interval: %d\n", update_interval);
            return 1;
        }
    }

    initialize_ncurses();

    size_t buffer_size = 4096;
    char *sensors_output = malloc(buffer_size);
    char *previous_output = malloc(buffer_size);

    if (sensors_output == NULL || previous_output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        endwin();
        return 1;
    }

    memset(previous_output, 0, buffer_size);

    // Initialize logger
    init_logger("sensor.log");

    while (1) {
        handle_resize();

        int height, width;
        getmaxyx(stdscr, height, width);

        read_sensors(&sensors_output, &buffer_size);

        if (has_sensor_data_changed(sensors_output, previous_output)) {
            strcpy(previous_output, sensors_output);
            display_sensors_output(sensors_output, height, width);

            // Example log entry (modify as per actual sensor output parsing)
            write_log(LOG_INFO, "device_name", "sensor_name", "sensor_value");
        }

        struct timespec req = {update_interval, 0};
        nanosleep(&req, NULL);

        if (getch() != ERR) {
            break;
        }

        // Periodic log rotation (e.g., every hour)
        static time_t last_rotation = 0;
        time_t now = time(NULL);
        if (difftime(now, last_rotation) >= 3600) { // Rotate log every hour
            rotate_log();
            last_rotation = now;
        }
    }

    free(sensors_output);
    free(previous_output);
    endwin();
    return 0;
}
