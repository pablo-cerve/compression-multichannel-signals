
#ifndef CPP_PROJECT_ARITHMETIC_MASK_DECODER_H
#define CPP_PROJECT_ARITHMETIC_MASK_DECODER_H

#include "constants.h"
#include "mask.h"
#include "decoder_common.h"

class ArithmeticMaskDecoder {

private:
    DecoderCommon* decoder;
    int data_columns_count;
    Mask* mask;

    void flush();
    std::vector<Mask*> callDecompress();

public:
    ArithmeticMaskDecoder(DecoderCommon* decoder_, int data_columns_count_);
    std::vector<Mask*> decode();

};

#endif //CPP_PROJECT_ARITHMETIC_MASK_DECODER_H
