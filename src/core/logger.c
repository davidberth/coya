#include "logger.h"
#include <time.h>

static time_t start_time;

void init_logger() { start_time = time(NULL); }

void log_message(log_level level, const char *message, const char *file,
                 int line, ...) {
  va_list args;
  va_start(args, line);

  time_t current_time = time(NULL);
  double elapsed_time = difftime(current_time, start_time);

  switch (level) {
  case LOG_INFO:
    printf("\033[0;32m[INFO]    ");
    break;
  case LOG_DEBUG:
    printf("\033[0;37m[DEBUG]   ");
    break;
  case LOG_WARNING:
    printf("\033[0;33m[WARNING] ");
    break;
  case LOG_ERROR:
    printf("\033[0;31m[ERROR]   ");
    break;
  default:
    break;
  }

  printf("[%.4f] %s:%d: ", elapsed_time, file, line);
  vprintf(message, args);
  printf("\033[0m\n");

  va_end(args);
}