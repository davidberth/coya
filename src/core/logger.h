#pragma once

#include <stdarg.h>
#include <stdio.h>


typedef enum {
    LOG_INFO = 0,
    LOG_DEBUG = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_FATAL = 4
} log_level;

void init_logger();

void log_message(log_level level, const char *message, const char *file,
                 int line, ...);

#define flog(message, ...)                                                     \
    log_message(LOG_FATAL, message, __FILE__, __LINE__, ##__VA_ARGS__);
#define elog(message, ...)                                                     \
    log_message(LOG_ERROR, message, __FILE__, __LINE__, ##__VA_ARGS__);
#define wlog(message, ...)                                                     \
    log_message(LOG_WARNING, message, __FILE__, __LINE__, ##__VA_ARGS__);
#define dlog(message, ...)                                                     \
    log_message(LOG_DEBUG, message, __FILE__, __LINE__, ##__VA_ARGS__);
#define ilog(message, ...)                                                     \
    log_message(LOG_INFO, message, __FILE__, __LINE__, ##__VA_ARGS__);
