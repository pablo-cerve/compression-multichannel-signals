
#include "coder_output.h"
#include "tests_utils.h"
#include "arithmetic_mask_coder.h"

CoderOutput::CoderOutput(CoderCommon* coder_){
    coder = coder_;
}

void CoderOutput::put_bit(bool bit){
    coder->codeBit(bit ? 1 : 0);
}

void CoderOutput::finishCoding(bool bit){
    // std::cout << "finishCoding" << std::endl;
    for (int i = 0; i < EOS_LENGTH; i++){
        put_bit(bit);
    }
    for (int i = 0; i < EOS_LENGTH; i++){
        put_bit(!bit);
    }
}
