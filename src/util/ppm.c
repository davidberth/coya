#include "core/logger.h"
#include "core/memory.h"
#include <stdio.h>
#include "ppm.h"

unsigned char *convert_to_RGBA(PPM *img, unsigned char alpha) {
    unsigned char *rgba = (unsigned char *)oalloc(
      img->width * img->height * 4, MEMORY_CATEGORY_TEXTURE);
    if (!rgba) {
        elog("memory allocation failed for RGBA conversion");
        return NULL;
    }

    // Optimized version using pointer arithmetic
    unsigned char *dst = (unsigned char *)rgba;
    for (int i = 0; i < img->width * img->height * 3; i += 3) {
        *dst++ = img->pixels[i];
        *dst++ = img->pixels[i + 1];
        *dst++ = img->pixels[i + 2];
        *dst++ = alpha;
    }

    return rgba;
}

PPM *load_ppm(const char *filename) {
    FILE *file;
    fopen_s(&file, filename, "rb");
    if (!file) {
        elog("unable to open file %s", filename);
        return NULL;
    }

    // read the header
    char format[3];
    if (fscanf_s(file, "%2s\n", format, (unsigned)sizeof(format)) != 1) {
        elog("invalid PPM format");
        fclose(file);
        return NULL;
    }

    // skip any comments
    int c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n')
            ;
        c = getc(file);
    }
    ungetc(c, file);

    // read width, height, max value
    int width, height, maxVal;
    if (fscanf_s(file, "%d %d\n%d\n", &width, &height, &maxVal) != 3) {
        elog("invalid PPM dimensions or max value");
        fclose(file);
        return NULL;
    }

    // allocate memory for the image
    PPM *img = (PPM *)oalloc(sizeof(PPM), MEMORY_CATEGORY_TEXTURE);

    if (!img) {
        elog("memory allocation failed");
        fclose(file);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->pixels =
      (unsigned char *)oalloc(width * height * 3, MEMORY_CATEGORY_TEXTURE);
    if (!img->pixels) {
        elog("memory allocation failed for pixels");
        ofree(img);
        fclose(file);
        return NULL;
    }

    // read the binary pixel data
    if (fread(img->pixels, sizeof(unsigned char), width * height * 3, file) !=
        width * height * 3) {
        elog("Error reading pixel data");
        ofree(img->pixels);
        ofree(img);
        fclose(file);
        return NULL;
    }

    fclose(file);

    unsigned char *RGBA = convert_to_RGBA(img, 255);
    ofree(img->pixels);
    img->pixels = RGBA;

    return img;
}

void free_ppm(PPM *ppm) {
    if (ppm) {
        ofree(ppm->pixels);
        ofree(ppm);
    }
}