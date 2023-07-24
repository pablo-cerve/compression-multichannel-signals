
#ifndef CPP_PROJECT_MATH_UTILS_H
#define CPP_PROJECT_MATH_UTILS_H

#include <cstdint>

class MathUtils {

public:
    static int bitLength(uint32_t value);
    static int windowSizeBitLength(uint32_t window_size);
    static int intAbsolute(int value);
    static double doubleAbsolute(double value);
    static int half(int first, int last);
};

#endif //CPP_PROJECT_MATH_UTILS_H
