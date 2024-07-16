#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

static FILE *log_file = NULL;

static void create_log_directory() {
    if (mkdir("log", 0777) && errno != EEXIST) {
        fprintf(stderr, "Error creating log directory.\n");
        exit(EXIT_FAILURE);
    }
}

static char* generate_log_filename(const char *filename) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    static char full_filename[256];
    strftime(full_filename, sizeof(full_filename) - 1, "log/%Y-%m-%d_%H-%M-%S_", t);
    strcat(full_filename, filename);
    return full_filename;
}

void init_logger(const char *filename) {
    create_log_directory();
    log_file = fopen(generate_log_filename(filename), "a");
    if (!log_file) {
        fprintf(stderr, "Error opening log file.\n");
        exit(EXIT_FAILURE);
    }
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void write_log(int severity, const char *device, const char *sensor, const char *value) {
    if (!log_file) {
        init_logger("sensor.log");
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", t);  // Ensure seconds are included

    const char *severity_str;
    switch (severity) {
        case LOG_INFO:
            severity_str = "INFO";
            break;
        case LOG_WARNING:
            severity_str = "WARNING";
            break;
        case LOG_ERROR:
            severity_str = "ERROR";
            break;
        default:
            severity_str = "UNKNOWN";
            break;
    }

    fprintf(log_file, "[%s]::[%s]::[%s-%s]: %s\n", timestamp, severity_str, device, sensor, value);
    fflush(log_file);
}
