#include "logger.h"      // Custom logging library header
#include <stdlib.h>      // Standard library for memory allocation, exit, etc.
#include <string.h>      // Library for handling string operations
#include <time.h>        // Library for handling date and time operations

// Static global variables for log file pointer and log filename
static FILE *log_fp = NULL;  // File pointer for the log file
static char log_filename[300];  // Buffer to store the log filename

/**
 * Initializes the logger by creating and opening a log file.
 * The log filename is based on the current date and time, with the provided logfile name appended.
 *
 * logfile: The base name for the log file (e.g., "sensor.log").
 */
void init_logger(const char *logfile) {
    time_t now = time(NULL);  // Get the current time
    struct tm *local_time = localtime(&now);  // Convert the time to local time structure

    // Format the log filename with the current date and time
    strftime(log_filename, sizeof(log_filename), "%Y-%m-%d_%H-%M-%S_", local_time);
    strncat(log_filename, logfile, sizeof(log_filename) - strlen(log_filename) - 1);  // Append the provided logfile name

    // Open the log file in append mode
    log_fp = fopen(log_filename, "a");
    if (log_fp == NULL) {
        // If the file cannot be opened, print an error and exit
        fprintf(stderr, "Failed to open log file: %s\n", log_filename);
        exit(EXIT_FAILURE);
    }
}

/**
 * Writes a log entry to the log file.
 *
 * severity: The severity level of the log entry (LOG_INFO, LOG_WARNING, LOG_ERROR).
 * device: The name of the device generating the log entry.
 * sensor: The name of the sensor generating the log entry.
 * value: The value or message to be logged.
 */
void write_log(int severity, const char *device, const char *sensor, const char *value) {
    if (log_fp == NULL) {
        // If the logger is not initialized, print an error
        fprintf(stderr, "Logger not initialized.\n");
        return;
    }

    time_t now = time(NULL);  // Get the current time
    struct tm *local_time = localtime(&now);  // Convert the time to local time structure
    char timestamp[30];  // Buffer to store the timestamp
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S%z", local_time);  // Format the timestamp

    // Convert the severity level to a string
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

    // Write the log entry to the log file
    fprintf(log_fp, "[%s]::[%s]::[%s]-%s: %s\n", timestamp, severity_str, device, sensor, value);
    fflush(log_fp);  // Ensure the log entry is written to the file immediately
}
