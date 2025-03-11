#include "camera.h"
#include "input/input.h"
#include "math/omath.h"

void camera_init(Camera *camera) {
    // initialize camera position at a distance from origin
    camera->radius = 5.0f;
    camera->euler = vec3_zero();
    camera->position = vec3_create(0.0f, 0.0f, camera->radius);
}

void camera_update(Camera *camera) {
    // rotation speeds (in radians per frame)
    float rotation_speed = 0.001f;
    float zoom_speed = 0.003f;

    // handle arrow key inputs for camera rotation
    if (get_key_state(INPUT_KEY_LEFT)) {
        camera->euler.y += rotation_speed;
    }
    if (get_key_state(INPUT_KEY_RIGHT)) {
        camera->euler.y -= rotation_speed;
    }
    if (get_key_state(INPUT_KEY_UP)) {
        camera->euler.x += rotation_speed;
        // limit pitch to avoid camera flipping
        if (camera->euler.x > O_HALFPI - 0.1f) {
            camera->euler.x = O_HALFPI - 0.1f;
        }
    }
    if (get_key_state(INPUT_KEY_DOWN)) {
        camera->euler.x -= rotation_speed;
        // limit pitch to avoid camera flipping
        if (camera->euler.x < -O_HALFPI + 0.1f) {
            camera->euler.x = -O_HALFPI + 0.1f;
        }
    }

    // handle zoom in/out with W/S keys
    if (get_key_state('W')) {
        camera->radius -= zoom_speed;
        if (camera->radius < 1.0f) {
            camera->radius = 1.0f;
        }
    }
    if (get_key_state('S')) {
        camera->radius += zoom_speed;
        if (camera->radius > 20.0f) {
            camera->radius = 20.0f;
        }
    }

    // calculate camera position based on spherical coordinates
    float sin_pitch = osin(camera->euler.x);
    float cos_pitch = ocos(camera->euler.x);
    float sin_yaw = osin(camera->euler.y);
    float cos_yaw = ocos(camera->euler.y);

    // convert from spherical to cartesian coordinates
    camera->position.x = camera->radius * cos_pitch * sin_yaw;
    camera->position.y = camera->radius * sin_pitch;
    camera->position.z = camera->radius * cos_pitch * cos_yaw;
}

mat4 camera_get_view_matrix(Camera *camera) {
    // create view matrix looking from camera position to origin
    vec3 center = vec3_zero();
    vec3 up = vec3_create(0.0f, 1.0f, 0.0f);
    return mat4_look_at(camera->position, center, up);
}