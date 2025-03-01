#pragma once

typedef union {
    float elem[2];
    struct {
        union {
            float x, u;
        };
        union {
            float y, v;
        };
    };
} vec2;

typedef struct {
    union {
        float elem[3];
        struct {
            union {
                float x, r;
            };
            union {
                float y, g;
            };
            union {
                float z, b;
            };
        };
    };
} vec3;

typedef struct {
    union {
        float elem[4];
        struct {
            union {
                float x, r;
            };
            union {
                float y, g;
            };
            union {
                float z, b;
            };
            union {
                float w, a;
            };
        };
    };
} vec4;

typedef float mat3x3[9];

typedef struct {
    vec3 position;
} Vertex3D;
