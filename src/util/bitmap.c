#include "core/logger.h"
#include "core/memory.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// read a bitmap file from a given path
Bitmap *load_bitmap(const char *path) {
    // open the file in binary read mode
    FILE *file;
    fopen_s(&file, path, "rb");
    if (!file) {
        elog("failed to open bitmap file");
        return nullptr;
    }
    // read the bitmap file header
    BITMAPFILEHEADER file_header;
    if (fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file) != 1) {
        elog("failed to read bitmap file header");
        fclose(file);
        return nullptr;
    }
    // check that the file is a valid bitmap
    if (file_header.bfType != 0x4d42) {
        elog("file is not a valid bitmap");
        fclose(file);
        return nullptr;
    }
    // read the bitmap info header
    BITMAPINFOHEADER info_header;
    if (fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file) != 1) {
        elog("failed to read bitmap info header");
        fclose(file);
        return nullptr;
    }
    // calculate image size if not provided
    int image_size = info_header.biSizeImage;
    if (image_size == 0) {
        image_size =
          ((info_header.biWidth * info_header.biBitCount + 31) / 32) * 4 *
          info_header.biHeight;
    }
    // allocate memory for pixel data
    unsigned char *data = oalloc(image_size, MEMORY_CATEGORY_TEXTURE);
    if (!data) {
        elog("failed to allocate memory for bitmap data");
        fclose(file);
        return nullptr;
    }
    // move file pointer to pixel data offset
    fseek(file, file_header.bfOffBits, SEEK_SET);
    // read pixel data
    if (fread(data, 1, image_size, file) != image_size) {
        elog("failed to read bitmap pixel data");
        ofree(data);
        fclose(file);
        return nullptr;
    }
    // close the file
    fclose(file);
    // allocate memory for Bitmap structure
    Bitmap *bitmap = oalloc(sizeof(Bitmap), MEMORY_CATEGORY_TEXTURE);
    if (!bitmap) {
        elog("failed to allocate memory for Bitmap structure");
        ofree(data);
        return nullptr;
    }
    // set bitmap width
    bitmap->width = info_header.biWidth;
    // set bitmap height
    bitmap->height = info_header.biHeight;
    // set bits per pixel
    bitmap->bits_per_pixel = info_header.biBitCount;
    // set bitmap data
    bitmap->data = data;
    // return the bitmap
    return bitmap;
}

// free the allocated bitmap
void free_bitmap(Bitmap *bitmap) {
    // check if bitmap is not nullptr
    if (bitmap) {
        // free the bitmap data
        ofree(bitmap->data);
        // free the bitmap structure
        ofree(bitmap);
    }
}
