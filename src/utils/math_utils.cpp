
#include "math_utils.h"
#include <cmath>
#include <cstdlib>
#include "assert.h"

/*
// SOURCE: https://stackoverflow.com/a/21192373/4547232
//
// Examples:
// MathUtils::bitLength(0) == 1 // 0
// MathUtils::bitLength(1) == 1 // 1
// MathUtils::bitLength(2) == 2 // 10
// MathUtils::bitLength(3) == 2 // 11
// MathUtils::bitLength(4) == 3 // 100
*/
int MathUtils::bitLength(uint32_t value){
    int bits = 0;
    for (int bit_test = 16; bit_test > 0; bit_test >>= 1){
        if (value >> bit_test != 0){
            bits += bit_test;
            value >>= bit_test;
        }
    }
    return bits + 1;
}

/*
// Examples:
// MathUtils::windowSizeBitLength(2) == 1 => 1 bit  to represent 2 possible lengths: [1, 2]
// MathUtils::windowSizeBitLength(3) == 2 => 2 bits to represent 3 possible lengths: [1, 2, 3]
// MathUtils::windowSizeBitLength(4) == 2 => 2 bits to represent 4 possible lengths: [1, 2, 3, 4]
*/
int MathUtils::windowSizeBitLength(uint32_t window_size){
    // window_size = 0 => it doesn't make sense
    // window_size = 1 => in this case the window has a unique size, so it doesn't make sense to code the size
    assert(window_size > 1);
    return bitLength(window_size - 1);
}

int MathUtils::intAbsolute(int value){
    return std::abs(value);
}

double MathUtils::doubleAbsolute(double value){
    return std::abs(value);
}

//
// PRE: first >= 0, last >= 0
//
int MathUtils::half(int first, int last){
    return (first + last) / 2;
}