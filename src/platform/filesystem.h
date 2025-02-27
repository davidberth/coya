#pragma once

typedef struct {
    void *handle;
    bool is_valid;
} FileHandle;

typedef enum {
    FILE_MODE_READ = 0x1,
    FILE_MODE_WRITE = 0x2,
} FileModes;

bool filesystem_exists(const char *path);

bool filesystem_open(
  const char *path, FileModes mode, bool binary, FileHandle *out_handle);

void filesystem_close(FileHandle *handle);

bool filesystem_read_line(FileHandle *handle, char **line_buf);

bool filesystem_write_line(FileHandle *handle, const char *text);

bool filesystem_read(FileHandle *handle, unsigned int data_size, void *out_data,
  unsigned int *out_bytes_read);

bool filesystem_read_all_bytes(
  FileHandle *handle, unsigned int **out_bytes, unsigned int *out_bytes_read);

bool filesystem_write(FileHandle *handle, unsigned int data_size,
  const void *data, unsigned int *out_bytes_written);
