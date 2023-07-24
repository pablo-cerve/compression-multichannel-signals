
#include "decoder_input.h"
#include <iostream>
#include "arithmetic_mask_coder.h"

DecoderInput::DecoderInput(BitStreamReader* input_file_){
    input_file = input_file_;
    current_burst = false; // doesn't matter
    int current_burst_count = -1;
    bool previous_burst = false;
}

int DecoderInput::get_bit(){
    int bit = input_file->getBit();
    if (current_burst_count == -1){ // first call
        current_burst = bit;
        current_burst_count = 1;
    }
    else if (current_burst == bit){
        current_burst_count++;
        if (previous_burst && current_burst_count == EOS_LENGTH){
//            std::cout << "THIS IS THE END..." << std::endl;
        }
    }
    else { // current_burst != bit
        if (current_burst_count == EOS_LENGTH) {
            if (!previous_burst) {
                previous_burst = true;
                current_burst = bit;
                current_burst_count = 1;
            }
        }
        else {
            previous_burst = false;
            current_burst = bit;
            current_burst_count = 1;
        }
    }
    return bit;
}

void DecoderInput::finishDecoding(){
    // std::cout << "finishDecoding" << std::endl;
    while (!(previous_burst && current_burst_count == EOS_LENGTH)){
        get_bit();
    }
}
