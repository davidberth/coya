#pragma once

// define the Bitmap structure
typedef struct Bitmap {
    int width;
    int height;
    unsigned short bits_per_pixel;
    unsigned char *data;
} Bitmap;

Bitmap *load_bitmap(const char *path);
void free_bitmap(Bitmap *bitmap);