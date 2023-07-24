
#ifndef CPP_PROJECT_ARITHMETIC_MASK_CODER_H
#define CPP_PROJECT_ARITHMETIC_MASK_CODER_H

#include "constants.h"

#define EOS_LENGTH 16 // used to mark the end of the stream

#include "mask.h"
#include "coder_common.h"

class ArithmeticMaskCoder {

private:
    CoderCommon* coder;
    int data_columns_count;

    void flush();
    std::vector<int> callCompress();

public:
    ArithmeticMaskCoder(CoderCommon* coder_, int data_columns_count_);
    std::vector<int> code();

};

#endif //CPP_PROJECT_ARITHMETIC_MASK_CODER_H
