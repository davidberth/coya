#pragma once

#include "math/types.h"
#include "resources/types.h"

typedef struct {
    mat4 projection;
    mat4 view;

    mat4 m_reserved0; // reserved for future use
    mat4 m_reserved1;

} GlobalUniformObject;

typedef struct {
    vec4 diffuse_color;
    vec4 reserved0;
    vec4 reserved1;
    vec4 reserved2;
} LocalUniformObject;

typedef struct {
    unsigned int object_id;
    mat4 model;
    Texture *textures[16];
} GeometryRenderData;