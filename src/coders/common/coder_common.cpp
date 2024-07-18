
#include "coder_common.h"

#include "header_coder.h"
#include "conversor.h"
#include "assert.h"
#include <math.h>
#include "coder_utils.h"

CoderCommon::CoderCommon(std::string coder_name_, CSVReader* input_csv_, BitStreamWriter* output_file_){
   coder_name = coder_name_;
   input_csv = input_csv_;
   output_file = output_file_;
   dataset = new Dataset();
}

bool CoderCommon::isCoder(std::string _coder_name){
    return coder_name == _coder_name;
}

void CoderCommon::codeCoderName() {
    int coder_value = Constants::getCoderValue(coder_name);
#if CHECKS
    assert(0 <= coder_value && coder_value < pow(2, 8));
#endif
    codeInt(coder_value, 8); // 8 bits for the coder_code
}

void CoderCommon::codeWindowParameter() {
#if CHECKS
    assert(1 <= window_size && window_size <= pow(2, 8));
#endif
    codeInt(window_size - 1, 8); // 8 bits for the window_size
}

Dataset* CoderCommon::code(){
    data_rows_count = HeaderCoder(input_csv, this).codeHeader(dataset);

    bool is_lossless = Constants::isLosslessCoder(coder_name);
    codeDataRows(is_lossless);

    dataset->printBits();
    closeFiles();
    return dataset;
}

//
// This method maps a value read in the csv file into an integer to be written in the output file.
//
int CoderCommon::codeValue(std::string x){
    std::string unmapped_x = CoderUtils::unmapValue(x, 0);

    if (Constants::isNoData(x)){ return dataset->nan(); }

    int x_int = Conversor::stringToInt(x);
#if CHECKS
    // check that the value is within the range
    if (!dataset->insideRange(x_int)) { throw std::invalid_argument(Conversor::intToString(x_int)); }
#endif
    return x_int + dataset->offset();
}

void CoderCommon::codeRaw(int value){
    output_file->pushInt(value, dataset->getBits());
}

void CoderCommon::codeBit(int bit){
    dataset->addBits(1);
    output_file->pushBit(bit);
}

void CoderCommon::codeBits(int bit, int times){
    dataset->addBits(times);
    output_file->pushBits(bit, times);
}

void CoderCommon::codeBool(bool bit){
    dataset->addBits(1);
    if (bit) { output_file->pushBit(1); }
    else     { output_file->pushBit(0); }
}

void CoderCommon::codeInt(int value, int bits){
    dataset->addBits(bits);
    output_file->pushInt(value, bits);
}

void CoderCommon::codeWindowLength(Window* window){
    codeInt(window->length - 1, window->window_size_bit_length);
}

void CoderCommon::codeUnary(int value){
    for(int i=0; i < value; i++) { codeBit(0); }
    codeBit(1);
}

void CoderCommon::codeUnaryInv(int value){
    for(int i=0; i < value; i++) { codeBit(1); }
    codeBit(0);
}

void CoderCommon::codeValueRaw(std::string x){
    int value;
    try {
        value = codeValue(x);
        // std::cout << "codeValue(" << x << ") = " << value << std::endl;
    }
    catch( const std::invalid_argument& e ){
        std::cout << "CoderCommon::codeValueRaw: " << e.what() << std::endl;
        delete input_csv;
        delete output_file;
        exit(-1);
    }
    codeRaw(value);
}

void CoderCommon::codeFloat(float x){
    dataset->addBits(sizeof(float)*8);
    output_file->pushFloat(x);
}

void CoderCommon::flushByte(){
    int remaining = output_file->flushByte();
    dataset->addBits(remaining);
}

void CoderCommon::closeFiles(){
    delete input_csv;
    delete output_file;
}
