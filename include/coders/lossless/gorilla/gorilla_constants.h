
#ifndef CPP_PROJECT_GORILLA_CONSTANTS_H
#define CPP_PROJECT_GORILLA_CONSTANTS_H

#include <cstdint>

class GorillaConstants {

public:
    // constants used by CoderGorilla and DecoderGorilla
    static constexpr uint32_t kLeadingZerosLengthBits = 5;
    static constexpr uint32_t kBlockSizeLengthBits = 6;
    static constexpr uint32_t kBlockSizeAdjustment = 1;
    static constexpr uint32_t kValue = 64;

};

#endif //CPP_PROJECT_GORILLA_CONSTANTS_H
