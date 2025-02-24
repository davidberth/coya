#pragma once

#include "types.h"

#define O_PI 3.14159265358979323846f
#define O_2PI 2.0f * O_PI
#define O_HALFPI 0.5f * O_PI
#define O_QUARTERPI 0.25f * O_PI
#define O_ONE_OVER_PI 1.0f / O_PI
#define O_ONE_OVER_2PI 1.0f / O_2PI
#define O_SQRT2 1.41421356237309504880f
#define O_DEG2RAD (O_PI / 180.0f)
#define O_RAD2DEG (180.0f / O_PI)

#define O_INF 1e30f
#define O_EPSILON 1e-8f

float osin(float x);
float ocos(float x);
float otan(float x);
float oacos(float x);
float osqrt(float x);
float oabs(float x);

inline bool is_power_of_2(unsigned int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

int orandom();
int orandom_in_range(int min, int max);

float orandomf();
float orandomf_in_range(float min, float max);

// vec2

inline vec2 vec2_create(float x, float y) {
    vec2 result;
    result.x = x;
    result.y = y;
    return result;
}

inline vec2 vec2_zero() { return (vec2){{0.0f, 0.0f}}; }
inline vec2 vec2_one() { return (vec2){{1.0f, 1.0f}}; }
inline vec2 vec2_right() { return (vec2){{1.0f, 0.0f}}; }
inline vec2 vec2_up() { return (vec2){{0.0f, 1.0f}}; }
inline vec2 vec2_left() { return (vec2){{-1.0f, 0.0f}}; }
inline vec2 vec2_down() { return (vec2){{0.0f, -1.0f}}; }

inline vec2 vec2_add(vec2 a, vec2 b) { return (vec2){{a.x + b.x, a.y + b.y}}; }
inline vec2 vec2_sub(vec2 a, vec2 b) { return (vec2){{a.x - b.x, a.y - b.y}}; }
inline vec2 vec2_mul(vec2 a, vec2 b) { return (vec2){{a.x * b.x, a.y * b.y}}; }
inline vec2 vec2_div(vec2 a, vec2 b) { return (vec2){{a.x / b.x, a.y / b.y}}; }

inline vec2 vec2_addf(vec2 a, float b) { return (vec2){{a.x + b, a.y + b}}; }
inline vec2 vec2_subf(vec2 a, float b) { return (vec2){{a.x - b, a.y - b}}; }
inline vec2 vec2_mulf(vec2 a, float b) { return (vec2){{a.x * b, a.y * b}}; }
inline vec2 vec2_divf(vec2 a, float b) {
    float inv = 1.0f / b;
    return (vec2){{a.x * inv, a.y * inv}};
}

inline float vec2_dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }

inline float vec2_length_sq(vec2 a) { return a.x * a.x + a.y * a.y; }
inline float vec2_length(vec2 a) { return osqrt(vec2_length_sq(a)); }

inline void vec2_normalize(vec2 *a) {
    float inv_len = 1.0f / vec2_length(*a);
    a->x *= inv_len;
    a->y *= inv_len;
}

inline vec2 vec2_normalize_copy(vec2 a) {
    float inv_len = 1.0f / vec2_length(a);
    a.x *= inv_len;
    a.y *= inv_len;
    return a;
}

inline bool vec2_comp(vec2 v0, vec2 v1, float tol) {
    return (oabs(v0.x - v1.x) < tol) && (oabs(v0.y - v1.y) < tol);
}

inline float vec2_distance(vec2 a, vec2 b) {
    return vec2_length(vec2_sub(a, b));
}

// vec 3

inline vec3 vec3_create(float x, float y, float z) {
    vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

inline vec3 vec3_zero() { return (vec3){{{0.0f, 0.0f, 0.0f}}}; }
inline vec3 vec3_one() { return (vec3){{{1.0f, 1.0f, 1.0f}}}; }
inline vec3 vec3_right() { return (vec3){{{1.0f, 0.0f, 0.0f}}}; }
inline vec3 vec3_up() { return (vec3){{{0.0f, 1.0f, 0.0f}}}; }
inline vec3 vec3_forward() { return (vec3){{{0.0f, 0.0f, 1.0f}}}; }

inline vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){{{a.x + b.x, a.y + b.y, a.z + b.z}}};
}

inline vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){{{a.x - b.x, a.y - b.y, a.z - b.z}}};
}

inline vec3 vec3_mul(vec3 a, vec3 b) {
    return (vec3){{{a.x * b.x, a.y * b.y, a.z * b.z}}};
}

inline vec3 vec3_div(vec3 a, vec3 b) {
    return (vec3){{{a.x / b.x, a.y / b.y, a.z / b.z}}};
}

inline vec3 vec3_addf(vec3 a, float b) {
    return (vec3){{{a.x + b, a.y + b, a.z + b}}};
}

inline vec3 vec3_subf(vec3 a, float b) {
    return (vec3){{{a.x - b, a.y - b, a.z - b}}};
}

inline vec3 vec3_mulf(vec3 a, float b) {
    return (vec3){{{a.x * b, a.y * b, a.z * b}}};
}

inline vec3 vec3_divf(vec3 a, float b) {
    float inv = 1.0f / b;
    return (vec3){{{a.x * inv, a.y * inv, a.z * inv}}};
}

inline float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
      {{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}}};
}

inline float vec3_length_sq(vec3 a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline float vec3_length(vec3 a) { return osqrt(vec3_length_sq(a)); }

inline vec3 vec3_normalize(vec3 *a) {
    float inv_len = 1.0f / vec3_length(*a);
    a->x *= inv_len;
    a->y *= inv_len;
    a->z *= inv_len;
}

inline vec3 vec3_normalize_copy(vec3 a) {
    float inv_len = 1.0f / vec3_length(a);
    a.x *= inv_len;
    a.y *= inv_len;
    a.z *= inv_len;
    return a;
}

inline bool vec3_comp(vec3 v0, vec3 v1, float tol) {
    return (oabs(v0.x - v1.x) < tol) && (oabs(v0.y - v1.y) < tol) &&
           (oabs(v0.z - v1.z) < tol);
}

inline float vec3_distance(vec3 a, vec3 b) {
    return vec3_length(vec3_sub(a, b));
}

// vec4

inline vec4 vec4_create(float x, float y, float z, float w) {
    vec4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

inline vec4 vec4_zero() { return (vec4){{{0.0f, 0.0f, 0.0f, 0.0f}}}; }
inline vec4 vec4_one() { return (vec4){{{1.0f, 1.0f, 1.0f, 1.0f}}}; }

inline vec4 vec4_add(vec4 a, vec4 b) {
    return (vec4){{{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}}};
}

inline vec4 vec4_sub(vec4 a, vec4 b) {
    return (vec4){{{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}}};
}

inline vec4 vec4_mul(vec4 a, vec4 b) {
    return (vec4){{{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}}};
}

inline vec4 vec4_div(vec4 a, vec4 b) {
    return (vec4){{{a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w}}};
}

inline vec4 vec4_addf(vec4 a, float b) {
    return (vec4){{{a.x + b, a.y + b, a.z + b, a.w + b}}};
}

inline vec4 vec4_subf(vec4 a, float b) {
    return (vec4){{{a.x - b, a.y - b, a.z - b, a.w - b}}};
}

inline vec4 vec4_mulf(vec4 a, float b) {
    return (vec4){{{a.x * b, a.y * b, a.z * b, a.w * b}}};
}

inline vec4 vec4_divf(vec4 a, float b) {
    float inv = 1.0f / b;
    return (vec4){{{a.x * inv, a.y * inv, a.z * inv, a.w * inv}}};
}

inline float vec4_dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float vec4_length_sq(vec4 a) {
    return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

inline float vec4_length(vec4 a) { return osqrt(vec4_length_sq(a)); }

inline void vec4_normalize(vec4 *a) {
    float inv_len = 1.0f / vec4_length(*a);
    a->x *= inv_len;
    a->y *= inv_len;
    a->z *= inv_len;
    a->w *= inv_len;
}

inline vec4 vec4_normalize_copy(vec4 a) {
    float inv_len = 1.0f / vec4_length(a);
    a.x *= inv_len;
    a.y *= inv_len;
    a.z *= inv_len;
    a.w *= inv_len;
    return a;
}

inline bool vec4_comp(vec4 v0, vec4 v1, float tol) {
    return (oabs(v0.x - v1.x) < tol) && (oabs(v0.y - v1.y) < tol) &&
           (oabs(v0.z - v1.z) < tol) && (oabs(v0.w - v1.w) < tol);
}

inline float vec4_distance(vec4 a, vec4 b) {
    return vec4_length(vec4_sub(a, b));
}

// conversions

inline vec2 vec2_from_vec3(vec3 a) { return (vec2){{a.x, a.y}}; }
inline vec2 vec2_from_vec4(vec4 a) { return (vec2){{a.x, a.y}}; }
inline vec3 vec3_from_vec2(vec2 a, float b) { return (vec3){{{a.x, a.y, b}}}; }
inline vec3 vec3_from_vec4(vec4 a) { return (vec3){{{a.x, a.y, a.z}}}; }
inline vec4 vec4_from_vec3(vec3 a, float b) {
    return (vec4){{{a.x, a.y, a.z, b}}};
}
inline vec4 vec4_from_vec2(vec2 a, float b, float c) {
    return (vec4){{{a.x, a.y, b, c}}};
}
