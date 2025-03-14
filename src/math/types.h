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

typedef union {
    float data[16];
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
} mat4;

typedef struct {
    vec3 position;
    vec2 texcoord;
} Vertex3D;
