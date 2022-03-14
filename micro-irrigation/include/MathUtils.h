#ifndef MATH_UTILS
#define MATH_UTILS

#include <Arduino.h>

namespace MathUtils {
    float mapFloat(double x, double in_min, double in_max, double out_min, double out_max);
}

#endif