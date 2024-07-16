#ifndef LOGGER_H
#define LOGGER_H

#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

void init_logger(const char *filename);
void close_logger();
void write_log(int severity, const char *device, const char *sensor, const char *value);

#endif
