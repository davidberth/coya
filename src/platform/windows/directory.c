#include "core/logger.h"
#include "platform/platform.h"
#include <windows.h>

char *platform_set_to_root_directory() {

    static char current_directory[MAX_PATH];
    // get current working directory
    if (GetCurrentDirectory(MAX_PATH, current_directory)) {
        size_t dir_length = strlen(current_directory);
        // check if current directory ends with "src"
        if (dir_length >= 3 &&
            strcmp(current_directory + dir_length - 3, "src") == 0) {
            int count = 0;
            char *last_slash = nullptr;
            // remove last 3 directories
            while (count < 3) {
                last_slash = strrchr(current_directory, '\\');
                if (last_slash == nullptr) {
                    break;
                }
                *last_slash = '\0';
                count++;
            }
            // change current directory
            SetCurrentDirectory(current_directory);
            ilog("navigated to root directory: %s", current_directory);
            GetCurrentDirectory(MAX_PATH, current_directory);
        }
    }

    return current_directory;
}