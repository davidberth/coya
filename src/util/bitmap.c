#include "core/logger.h"
#include "core/memory.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// read a bitmap file from a given path and convert to RGBA format
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

    // calculate the stride (bytes per row) with padding to 4-byte boundary
    int src_stride =
      ((info_header.biWidth * info_header.biBitCount + 31) / 32) * 4;
    int image_size = src_stride * info_header.biHeight;

    // allocate memory for raw bitmap pixel data
    unsigned char *raw_data = oalloc(image_size, MEMORY_CATEGORY_TEXTURE);
    if (!raw_data) {
        elog("failed to allocate memory for raw bitmap data");
        fclose(file);
        return nullptr;
    }

    // move file pointer to pixel data offset
    fseek(file, file_header.bfOffBits, SEEK_SET);

    // read pixel data
    if (fread(raw_data, 1, image_size, file) != image_size) {
        elog("failed to read bitmap pixel data");
        ofree(raw_data);
        fclose(file);
        return nullptr;
    }

    // close the file
    fclose(file);

    // allocate memory for Vulkan-friendly RGBA format
    int rgba_size = info_header.biWidth * info_header.biHeight * 4;
    unsigned char *rgba_data = oalloc(rgba_size, MEMORY_CATEGORY_TEXTURE);
    if (!rgba_data) {
        elog("failed to allocate memory for RGBA data");
        ofree(raw_data);
        return nullptr;
    }

    // convert bitmap data to RGBA
    int width = info_header.biWidth;
    int height = info_header.biHeight;
    int bits_per_pixel = info_header.biBitCount;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // bitmap rows are stored bottom-to-top, so flip y coordinate
            int src_y = height - 1 - y;
            int src_pos = src_y * src_stride + x * (bits_per_pixel / 8);
            int dst_pos = (y * width + x) * 4; // RGBA = 4 bytes per pixel

            if (bits_per_pixel == 24) {                         // BGR
                rgba_data[dst_pos + 0] = raw_data[src_pos + 2]; // R from B
                rgba_data[dst_pos + 1] = raw_data[src_pos + 1]; // G from G
                rgba_data[dst_pos + 2] = raw_data[src_pos + 0]; // B from R
                rgba_data[dst_pos + 3] = 255;  // A (default opaque)
            } else if (bits_per_pixel == 32) { // BGRA
                rgba_data[dst_pos + 0] = raw_data[src_pos + 2]; // R from B
                rgba_data[dst_pos + 1] = raw_data[src_pos + 1]; // G from G
                rgba_data[dst_pos + 2] = raw_data[src_pos + 0]; // B from R
                rgba_data[dst_pos + 3] = raw_data[src_pos + 3]; // A from A
            }
        }
    }

    // free the raw bitmap data
    ofree(raw_data);

    // allocate memory for Bitmap structure
    Bitmap *bitmap = oalloc(sizeof(Bitmap), MEMORY_CATEGORY_TEXTURE);
    if (!bitmap) {
        elog("failed to allocate memory for Bitmap structure");
        ofree(rgba_data);
        return nullptr;
    }

    // set bitmap properties
    bitmap->width = width;
    bitmap->height = height;
    bitmap->bits_per_pixel = 32; // Always 32bpp for RGBA
    bitmap->data = rgba_data;

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
