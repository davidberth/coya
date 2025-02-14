#pragma once

#include <assert.h>

#define vk_check(expr)                                                         \
    {                                                                          \
        assert(expr == VK_SUCCESS);                                            \
    }