#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>   // Standard I/O library for file operations
#include <time.h>    // Library for handling date and time operations

// Define constants for log severity levels
#define LOG_INFO    0
#define LOG_WARNING 1
#define LOG_ERROR   2

/**
 * Initializes the logger by creating and opening a log file.
 *
 * logfile: The base name for the log file (e.g., "sensor.log").
 */
void init_logger(const char *logfile);

/**
 * Writes a log entry to the log file.
 *
 * severity: The severity level of the log entry (LOG_INFO, LOG_WARNING, LOG_ERROR).
 * device: The name of the device generating the log entry.
 * sensor: The name of the sensor generating the log entry.
 * value: The value or message to be logged.
 */
void write_log(int severity, const char *device, const char *sensor, const char *value);

#endif // LOGGER_H
