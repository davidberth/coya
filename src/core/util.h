#pragma once

#define oclamp(value, min, max)                                                \
    (value <= min) ? min : (value >= max) ? max : value