#include "logger.h"
#include <sys\timeb.h>

static struct timeb start;

void init_logger() { ftime(&start); }

void log_message(log_level level, const char *message, const char *file,
                 int line, ...) {
    va_list args;
    va_start(args, line);
    struct timeb current;
    ftime(&current);
    float time_difference = (current.time - start.time) +
                            (current.millitm - start.millitm) / 1000.0f;

    static const char *level_strings[] = {
        "\033[0;32m[INFO]    ", "[DEBUG]   ", "\033[0;33m[WARNING] ",
        "\033[0;31m[ERROR]   ", "\033[0;31m[FATAL]   "};
    printf("%s", level_strings[level]);

    printf("[%010.3f] %s:%04d:\t", time_difference, file, line);
    vprintf(message, args);
    printf("\033[0m\n");

    va_end(args);
}