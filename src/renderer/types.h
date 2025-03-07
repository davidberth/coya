#include "math/types.h"

typedef struct {
    mat4 projection;
    mat4 view;

    mat4 m_reserved0; // reserved for future use
    mat4 m_reserved1;

} GlobalUniformObject;