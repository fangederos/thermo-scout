#include "logger.h"  // Header file for this module
#include <stdio.h>   // Standard I/O library
#include <stdlib.h>  // Standard library for memory allocation and conversion functions
#include <string.h>  // String handling functions
#include <time.h>    // Time handling functions
#include <sys/stat.h>  // File status functions
#include <errno.h>   // Error number definitions

static FILE *log_file = NULL;  // Static variable to hold the file pointer for the log file

// Function to create a log directory if it doesn't exist
static void create_log_directory() {
    // Attempt to create the 'log' directory with read/write/execute permissions
    if (mkdir("log", 0777) && errno != EEXIST) {
        fprintf(stderr, "Error creating log directory.\n");  // Print error message if directory creation fails
        exit(EXIT_FAILURE);  // Exit the program with failure status
    }
}

// Function to generate a log filename with a timestamp
static char* generate_log_filename(const char *filename) {
    time_t now = time(NULL);  // Get the current time
    struct tm *t = localtime(&now);  // Convert the time to local time format
    static char full_filename[256];  // Buffer to hold the full filename
    strftime(full_filename, sizeof(full_filename) - 1, "log/%Y-%m-%d_%H-%M-%S_", t);  // Format the timestamp
    strcat(full_filename, filename);  // Append the base filename
    return full_filename;  // Return the generated filename
}

// Function to initialize the logger
void init_logger(const char *filename) {
    create_log_directory();  // Ensure the log directory exists
    log_file = fopen(generate_log_filename(filename), "a");  // Open the log file in append mode
    if (!log_file) {  // Check if the file was opened successfully
        fprintf(stderr, "Error opening log file.\n");  // Print error message if file opening fails
        exit(EXIT_FAILURE);  // Exit the program with failure status
    }
}

// Function to close the logger
void close_logger() {
    if (log_file) {  // Check if the log file is open
        fclose(log_file);  // Close the log file
        log_file = NULL;  // Reset the file pointer
    }
}

// Function to write a log entry
void write_log(int severity, const char *device, const char *sensor, const char *value) {
    if (!log_file) {  // Check if the log file is open
        init_logger("sensor.log");  // Initialize the logger with a default filename if not open
    }
    time_t now = time(NULL);  // Get the current time
    struct tm *t = localtime(&now);  // Convert the time to local time format
    char timestamp[20];  // Buffer to hold the timestamp
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", t);  // Format the timestamp to include seconds

    // Determine the severity level as a string
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
    fprintf(log_file, "[%s]::[%s]::[%s-%s]: %s\n", timestamp, severity_str, device, sensor, value);
    fflush(log_file);  // Ensure the log entry is written to disk
}
