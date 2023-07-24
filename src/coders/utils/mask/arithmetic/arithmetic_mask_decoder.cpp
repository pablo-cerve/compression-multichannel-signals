
#include "arithmetic_mask_decoder.h"
#include "decompressor.h"
#include "decoder_input.h"
#include "decoder_output.h"
#include "modelKT.h"

ArithmeticMaskDecoder::ArithmeticMaskDecoder(DecoderCommon* decoder_, int data_columns_count_){
    decoder = decoder_;
    data_columns_count = data_columns_count_;
}

std::vector<Mask*> ArithmeticMaskDecoder::decode(){
    flush();
    std::vector<Mask*> masks_vector = callDecompress();
    flush();
    return masks_vector;
}

void ArithmeticMaskDecoder::flush(){
    // std::cout << "D1 >> decoder->flushByte();" << std::endl;
    decoder->flushByte();
    // std::cout << "D1 >> decoder->flushByte();" << std::endl;
}

std::vector<Mask*> ArithmeticMaskDecoder::callDecompress(){
    DecoderInput input(decoder->input_file);
    DecoderOutput output(data_columns_count, decoder->data_rows_count);
    modelKT<int, 16, 14> model;
    decompress(input, output, model);
    return output.masks_vector;
}
