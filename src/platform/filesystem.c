#include "filesystem.h"

#include "core/logger.h"
#include "core/memory.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

bool filesystem_exists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

bool filesystem_open(
  const char *path, FileModes mode, bool binary, FileHandle *out_handle) {
    out_handle->is_valid = -false;
    out_handle->handle = 0;
    char mode_str[3];
    if ((mode & FILE_MODE_READ) != 0 && (mode & FILE_MODE_WRITE) != 0) {
        strcpy_s(mode_str, 3, binary ? "w+b" : "w+");
    } else if ((mode & FILE_MODE_READ) != 0) {
        strcpy_s(mode_str, 3, binary ? "rb" : "r");
    } else if ((mode & FILE_MODE_WRITE) != 0) {
        strcpy_s(mode_str, 3, binary ? "wb" : "w");
    } else {
        elog("Invalid file mode.");
        return false;
    }

    FILE *file = fopen(path, mode_str);
    if (!file) {
        flog("Failed to open file '%s'", path);
        return false;
    }
    out_handle->handle = file;
    out_handle->is_valid = true;
    return true;
}

void filesystem_close(FileHandle *handle) {
    if (handle->is_valid) {
        fclose((FILE *)handle->handle);
        handle->is_valid = false;
        handle->handle = 0;
    }
}
constexpr unsigned int buffer_size = 1024;
bool filesystem_read_line(FileHandle *handle, char **line_buf) {
    if (handle->handle) {
        char buffer[buffer_size];
        if (fgets(buffer, 32000, (FILE *)handle->handle)) {
            unsigned int length = strlen(buffer);
            *line_buf =
              oalloc((sizeof(char) * length) + 1, MEMORY_CATEGORY_STRING);
            strcpy_s(*line_buf, length + 1, buffer);
            return true;
        }
    }
    return false;
}

bool filesystem_write_line(FileHandle *handle, const char *text) {
    if (handle->handle) {
        int result = fputs(text, (FILE *)handle->handle);
        if (result != EOF) {
            result = fputc('\n', (FILE *)handle->handle);
        }
        fflush((FILE *)handle->handle);
        return result != EOF;
    }
    return false;
}

bool filesystem_read(FileHandle *handle, unsigned int data_size, void *out_data,
  unsigned int *out_bytes_read) {
    if (handle->handle && out_data) {
        *out_bytes_read = fread(out_data, 1, data_size, (FILE *)handle->handle);
        if (*out_bytes_read != data_size) {
            return false;
        }
        return true;
    }
    return false;
}

bool filesystem_read_all_bytes(
  FileHandle *handle, unsigned int **out_bytes, unsigned int *out_bytes_read) {
    if (handle->handle) {
        fseek((FILE *)handle->handle, 0, SEEK_END);
        unsigned int size = ftell((FILE *)handle->handle);
        rewind((FILE *)handle->handle);

        *out_bytes =
          oalloc(sizeof(unsigned int) * size, MEMORY_CATEGORY_STRING);
        *out_bytes_read = fread(*out_bytes, 1, size, (FILE *)handle->handle);
        if (*out_bytes_read != size) {
            return false;
        }
        return true;
    }
    return false;
}

bool filesystem_write(FileHandle *handle, unsigned int data_size,
  const void *data, unsigned int *out_bytes_written) {
    if (handle->handle) {
        *out_bytes_written = fwrite(data, 1, data_size, (FILE *)handle->handle);
        if (*out_bytes_written != data_size) {
            return false;
        }
        fflush((FILE *)handle->handle);
        return true;
    }
    return false;
}
