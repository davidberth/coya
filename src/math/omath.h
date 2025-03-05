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

inline void vec3_normalize(vec3 *a) {
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

mat4 mat4_identity() {
    mat4 result = {0};
    result.m00 = 1.0f;
    result.m11 = 1.0f;
    result.m22 = 1.0f;
    result.m33 = 1.0f;
    return result;
}

mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 result;
    result.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30;
    result.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31;
    result.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32;
    result.m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33;

    result.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30;
    result.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    result.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    result.m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;

    result.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30;
    result.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    result.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    result.m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;

    result.m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30;
    result.m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    result.m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    result.m33 = a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
    return result;
}

mat4 mat4_orthographic(
  float left, float right, float bottom, float top, float near, float far) {
    mat4 result = mat4_identity();

    float lr = 1.0f / (left - right);
    float bt = 1.0f / (bottom - top);
    float nf = 1.0f / (near - far);

    result.m00 = -2.0f * lr;
    result.m11 = -2.0f * bt;
    result.m22 = 2.0f * nf;

    result.m03 = (left + right) * lr;
    result.m13 = (top + bottom) * bt;
    result.m23 = (far + near) * nf;

    return result;
}

mat4 mat4_perspective(float fov, float aspect, float near, float far) {
    mat4 result = {0};

    float tan_half_fov = otan(fov * 0.5f);
    result.m00 = 1.0f / (aspect * tan_half_fov);
    result.m11 = 1.0f / tan_half_fov;
    result.m22 = (far + near) / (near - far);
    result.m23 = -1.0f;
    result.m32 = (2.0f * far * near) / (near - far);

    return result;
}

mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 f = vec3_normalize_copy(vec3_sub(center, eye));
    vec3 r = vec3_normalize_copy(vec3_cross(f, up));
    vec3 u = vec3_cross(r, f);

    mat4 result = mat4_identity();
    result.m00 = r.x;
    result.m01 = r.y;
    result.m02 = r.z;
    result.m10 = u.x;
    result.m11 = u.y;
    result.m12 = u.z;
    result.m20 = -f.x;
    result.m21 = -f.y;
    result.m22 = -f.z;
    result.m30 = -vec3_dot(r, eye);
    result.m31 = -vec3_dot(u, eye);
    result.m32 = vec3_dot(f, eye);
    return result;
}

// calculate the determinant of a 3x3 submatrix
inline float mat4_det3x3(float a, float b, float c, float d, float e, float f,
  float g, float h, float i) {
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

mat4 mat4_transpose(mat4 m) {
    mat4 result;
    result.m00 = m.m00;
    result.m01 = m.m10;
    result.m02 = m.m20;
    result.m03 = m.m30;
    result.m10 = m.m01;
    result.m11 = m.m11;
    result.m12 = m.m21;
    result.m13 = m.m31;
    result.m20 = m.m02;
    result.m21 = m.m12;
    result.m22 = m.m22;
    result.m23 = m.m32;
    result.m30 = m.m03;
    result.m31 = m.m13;
    result.m32 = m.m23;
    result.m33 = m.m33;
    return result;
}

mat4 mat4_inverse(mat4 m) {
    // calculate cofactors for each element
    float c00 = mat4_det3x3(
      m.m11, m.m12, m.m13, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33);
    float c01 = -mat4_det3x3(
      m.m10, m.m12, m.m13, m.m20, m.m22, m.m23, m.m30, m.m32, m.m33);
    float c02 = mat4_det3x3(
      m.m10, m.m11, m.m13, m.m20, m.m21, m.m23, m.m30, m.m31, m.m33);
    float c03 = -mat4_det3x3(
      m.m10, m.m11, m.m12, m.m20, m.m21, m.m22, m.m30, m.m31, m.m32);

    // calculate determinant using first row of cofactors
    float det = m.m00 * c00 + m.m01 * c01 + m.m02 * c02 + m.m03 * c03;

    // check for singular matrix (determinant close to zero)
    if (oabs(det) < O_EPSILON) {
        // return identity if matrix is singular
        return mat4_identity();
    }

    // calculate remaining cofactors
    float c10 = -mat4_det3x3(
      m.m01, m.m02, m.m03, m.m21, m.m22, m.m23, m.m31, m.m32, m.m33);
    float c11 = mat4_det3x3(
      m.m00, m.m02, m.m03, m.m20, m.m22, m.m23, m.m30, m.m32, m.m33);
    float c12 = -mat4_det3x3(
      m.m00, m.m01, m.m03, m.m20, m.m21, m.m23, m.m30, m.m31, m.m33);
    float c13 = mat4_det3x3(
      m.m00, m.m01, m.m02, m.m20, m.m21, m.m22, m.m30, m.m31, m.m32);

    float c20 = mat4_det3x3(
      m.m01, m.m02, m.m03, m.m11, m.m12, m.m13, m.m31, m.m32, m.m33);
    float c21 = -mat4_det3x3(
      m.m00, m.m02, m.m03, m.m10, m.m12, m.m13, m.m30, m.m32, m.m33);
    float c22 = mat4_det3x3(
      m.m00, m.m01, m.m03, m.m10, m.m11, m.m13, m.m30, m.m31, m.m33);
    float c23 = -mat4_det3x3(
      m.m00, m.m01, m.m02, m.m10, m.m11, m.m12, m.m30, m.m31, m.m32);

    float c30 = -mat4_det3x3(
      m.m01, m.m02, m.m03, m.m11, m.m12, m.m13, m.m21, m.m22, m.m23);
    float c31 = mat4_det3x3(
      m.m00, m.m02, m.m03, m.m10, m.m12, m.m13, m.m20, m.m22, m.m23);
    float c32 = -mat4_det3x3(
      m.m00, m.m01, m.m03, m.m10, m.m11, m.m13, m.m20, m.m21, m.m23);
    float c33 = mat4_det3x3(
      m.m00, m.m01, m.m02, m.m10, m.m11, m.m12, m.m20, m.m21, m.m22);

    // create adjoint matrix (transpose of cofactor matrix)
    mat4 result;
    result.m00 = c00;
    result.m01 = c10;
    result.m02 = c20;
    result.m03 = c30;
    result.m10 = c01;
    result.m11 = c11;
    result.m12 = c21;
    result.m13 = c31;
    result.m20 = c02;
    result.m21 = c12;
    result.m22 = c22;
    result.m23 = c32;
    result.m30 = c03;
    result.m31 = c13;
    result.m32 = c23;
    result.m33 = c33;

    // multiply by reciprocal of determinant
    float inv_det = 1.0f / det;

    result.m00 *= inv_det;
    result.m01 *= inv_det;
    result.m02 *= inv_det;
    result.m03 *= inv_det;
    result.m10 *= inv_det;
    result.m11 *= inv_det;
    result.m12 *= inv_det;
    result.m13 *= inv_det;
    result.m20 *= inv_det;
    result.m21 *= inv_det;
    result.m22 *= inv_det;
    result.m23 *= inv_det;
    result.m30 *= inv_det;
    result.m31 *= inv_det;
    result.m32 *= inv_det;
    result.m33 *= inv_det;

    return result;
}

mat4 mat4_translation(vec3 v) {
    mat4 result = mat4_identity();
    result.m03 = v.x;
    result.m13 = v.y;
    result.m23 = v.z;
    return result;
}

mat4 mat4_scale(vec3 v) {
    mat4 result = mat4_identity();
    result.m00 = v.x;
    result.m11 = v.y;
    result.m22 = v.z;
    return result;
}

mat4 mat4_rotation_x(float angle) {
    mat4 result = mat4_identity();
    float c = ocos(angle);
    float s = osin(angle);
    result.m11 = c;
    result.m12 = -s;
    result.m21 = s;
    result.m22 = c;
    return result;
}

mat4 mat4_rotation_y(float angle) {
    mat4 result = mat4_identity();
    float c = ocos(angle);
    float s = osin(angle);
    result.m00 = c;
    result.m02 = s;
    result.m20 = -s;
    result.m22 = c;
    return result;
}

mat4 mat4_rotation_z(float angle) {
    mat4 result = mat4_identity();
    float c = ocos(angle);
    float s = osin(angle);
    result.m00 = c;
    result.m01 = -s;
    result.m10 = s;
    result.m11 = c;
    return result;
}

mat4 mat4_rotation_xyz(float x, float y, float z) {
    return mat4_mul(
      mat4_mul(mat4_rotation_x(x), mat4_rotation_y(y)), mat4_rotation_z(z));
}

vec3 mat4_forward(mat4 m) {
    return vec3_normalize_copy(vec3_create(-m.m20, -m.m21, -m.m22));
}

vec3 mat4_right(mat4 m) {
    return vec3_normalize_copy(vec3_create(m.m00, m.m01, m.m02));
}

vec3 mat4_up(mat4 m) {
    return vec3_normalize_copy(vec3_create(m.m10, m.m11, m.m12));
}

vec3 mat4_backward(mat4 m) {
    return vec3_normalize_copy(vec3_create(m.m20, m.m21, m.m22));
}

vec3 mat4_left(mat4 m) {
    return vec3_normalize_copy(vec3_create(-m.m00, -m.m01, -m.m02));
}

vec3 mat4_down(mat4 m) {
    return vec3_normalize_copy(vec3_create(-m.m10, -m.m11, -m.m12));
}