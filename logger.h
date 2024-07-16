#ifndef LOGGER_H
#define LOGGER_H

#define LOG_INFO 1     // Define log level for informational messages
#define LOG_WARNING 2  // Define log level for warning messages
#define LOG_ERROR 3    // Define log level for error messages

// Function prototypes for logging functionality
void init_logger(const char *filename);  // Initialize the logger
void close_logger();                     // Close the logger
void write_log(int severity, const char *device, const char *sensor, const char *value);  // Write a log entry

#endif // LOGGER_H
