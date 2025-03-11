#pragma once

#include "math/types.h"

typedef struct {
    unsigned int id;
    unsigned int width;
    unsigned int height;
    unsigned short int channel_count;
    bool has_alpha;
    unsigned int generation;
    void *internal_data;
} Texture;
