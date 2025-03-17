#pragma once

#include "math/types.h"

// camera state structure
typedef struct {
    vec3 position;
    vec3 euler;
    float radius;
} Camera;

// initialize camera at default position
void camera_init(Camera *camera);

// update camera position and orientation based on input
void camera_update(Camera *camera, double delta_time);

// create a view matrix from the camera
mat4 camera_get_view_matrix(Camera *camera);