#include "logger.h"
#include "core/logger.h"
#include <string.h>
#include <sys\timeb.h>
#include <stdio.h>

static struct timeb start;
#ifndef _DDEBUG
static log_level current_level = LOG_INFO;
#else
static log_level current_level = LOG_MEMORY;
#endif
void init_logger() { ftime(&start); }

void set_logger_level(log_level level) {}

void log_message(
  log_level level, const char *message, const char *file, int line, ...) {

    if (level < current_level) {
        return;
    }
    va_list args;
    va_start(args, line);
    struct timeb current;
    ftime(&current);
    float time_difference =
      (current.time - start.time) + (current.millitm - start.millitm) / 1000.0f;

    static const char *level_strings[] = {"\033[0;32m[INFO]    ",
      "\033[0;34m[MEMORY]  ", "[DEBUG]   ", "\033[0;33m[WARNING] ",
      "\033[0;31m[ERROR]   ", "\033[0;31m[FATAL]   "};
    printf("%s", level_strings[level]);

    size_t file_length = strlen(file);
    const char *short_file = file_length > 16 ? file + file_length - 16 : file;

    printf("[%010.3f] %s:%04d:\t", time_difference, short_file, line);
    vprintf(message, args);
    printf("\033[0m\n");

    va_end(args);
}