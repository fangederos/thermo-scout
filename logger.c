#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

FILE *log_file = NULL;

void init_logger(const char *filename) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char full_filename[256];
    strftime(full_filename, sizeof(full_filename) - 1, "log/%Y-%m-%d_%H-%M-%S_", t);
    strcat(full_filename, filename);
    
    // Ensure the 'log' directory exists
    if (mkdir("log", 0777) && errno != EEXIST) {
        fprintf(stderr, "Error creating log directory.\n");
        exit(EXIT_FAILURE);
    }
    
    log_file = fopen(full_filename, "a");
    if (!log_file) {
        fprintf(stderr, "Error opening log file.\n");
        exit(EXIT_FAILURE);
    }
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
    }
}

void write_log(int severity, const char *device, const char *sensor, const char *value) {
    if (!log_file) {
        init_logger("sensor.log");
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp) - 1, "%Y-%m-%dT%H:%M:%S", t);

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
