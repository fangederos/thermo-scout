#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static FILE *log_fp = NULL;
static char log_filename[300];

void init_logger(const char *logfile) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    strftime(log_filename, sizeof(log_filename), "%Y-%m-%d_%H-%M-%S_", local_time);
    strncat(log_filename, logfile, sizeof(log_filename) - strlen(log_filename) - 1);

    log_fp = fopen(log_filename, "a");
    if (log_fp == NULL) {
        fprintf(stderr, "Failed to open log file: %s\n", log_filename);
        exit(EXIT_FAILURE);
    }
}

void write_log(int severity, const char *device, const char *sensor, const char *value) {
    if (log_fp == NULL) {
        fprintf(stderr, "Logger not initialized.\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S%z", local_time);

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

    fprintf(log_fp, "[%s]::[%s]::[%s]-%s: %s\n", timestamp, severity_str, device, sensor, value);
    fflush(log_fp);
}

void rotate_log() {
    if (log_fp == NULL) {
        fprintf(stderr, "Logger not initialized.\n");
        return;
    }

    fclose(log_fp);

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    strftime(log_filename, sizeof(log_filename), "%Y-%m-%d_%H-%M-%S_", local_time);
    strncat(log_filename, "_sensor.log", sizeof(log_filename) - strlen(log_filename) - 1);

    log_fp = fopen(log_filename, "a");
    if (log_fp == NULL) {
        fprintf(stderr, "Failed to open new log file: %s\n", log_filename);
        exit(EXIT_FAILURE);
    }
}
