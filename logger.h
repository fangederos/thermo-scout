#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#define LOG_INFO    0
#define LOG_WARNING 1
#define LOG_ERROR   2

void init_logger(const char *logfile);
void write_log(int severity, const char *device, const char *sensor, const char *value);
void rotate_log();

#endif // LOGGER_H

