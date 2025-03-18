#pragma once

typedef struct {
    int width;
    int height;
    unsigned char *pixels;
} PPM;

PPM *load_ppm(const char *path);
void free_ppm(PPM *ppm);