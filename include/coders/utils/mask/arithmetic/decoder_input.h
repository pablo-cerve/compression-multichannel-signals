
#ifndef CPP_PROJECT_DECODER_INPUT_H
#define CPP_PROJECT_DECODER_INPUT_H

#include "constants.h"
#include "bit_stream_reader.h"

class DecoderInput {

private:
    BitStreamReader* input_file;
    bool current_burst;
    int current_burst_count;
    bool previous_burst;

public:
    DecoderInput(BitStreamReader* input_file_);
    int get_bit();
    void finishDecoding();
};

#endif //CPP_PROJECT_DECODER_INPUT_H
