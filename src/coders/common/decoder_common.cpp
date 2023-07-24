
#include "decoder_common.h"

#include "header_decoder.h"
#include "conversor.h"
#include "math_utils.h"

DecoderCommon::DecoderCommon(std::string coder_name_, BitStreamReader* input_file_, CSVWriter* output_csv_){
    coder_name = coder_name_;
    input_file = input_file_;
    output_csv = output_csv_;
}

std::string DecoderCommon::decodeCoderName(BitStreamReader* input_file){
    int coder_value = input_file->getInt(8); // 8 bits for the coder_value
    std::string coder_name = Constants::getCoderName(coder_value);
    return coder_name;
}

void DecoderCommon::decodeWindowParameter(){
    int window_parameter = input_file->getInt(8) + 1; // 8 bits for the window_size
    window_size = window_parameter;
    window_size_bit_length = MathUtils::windowSizeBitLength(window_size);
}

void DecoderCommon::decode(){
    dataset = HeaderDecoder(input_file, output_csv).decodeHeader(data_rows_count);

    bool coder_base = coder_name == "Base";
    decodeDataRows(coder_base);

    closeFiles();
}

std::string DecoderCommon::decodeValue(int y){
    if (y == dataset->nan()) { return Constants::NO_DATA; }

    y -= dataset->offset();
    if (dataset->insideRange(y)) { return Conversor::intToString(y); }

    throw std::invalid_argument(Conversor::intToString(y));
}

int DecoderCommon::decodeRaw(){
    return input_file->getInt(dataset->bits());
}

void DecoderCommon::setWindowSizeBitLength(int window_size_bit_length_){
    window_size_bit_length = window_size_bit_length_;
}

bool DecoderCommon::decodeBool(){
    if (input_file->getBit()) { return true; } else { return false; }
}

int DecoderCommon::decodeInt(int bits){
    return (input_file->getInt(bits));
}

int DecoderCommon::decodeWindowLength(int window_size_bit_length){
    return input_file->getInt(window_size_bit_length) + 1;
}

int DecoderCommon::decodeWindowLength(){
    return decodeWindowLength(window_size_bit_length);
}

int DecoderCommon::decodeUnary(){
    int value = 0;
    while (!decodeBool()) { value++; }
    return value;
}

std::string DecoderCommon::decodeValueRaw(){
    int value = decodeRaw();
    std::string coded_value;
    try {
        coded_value = decodeValue(value);
    }
    catch( const std::invalid_argument& e ){
        std::cout << "DecoderCommon::decodeValueRaw: " << e.what() << std::endl;
        delete input_file;
        delete output_csv;
        exit(-1);
    }
    return coded_value;
}

float DecoderCommon::decodeFloat(){
    return input_file->getFloat();
}

void DecoderCommon::flushByte(){
    input_file->flushByte();
}

void DecoderCommon::transposeMatrix(int data_rows_count_, std::vector<std::vector<std::string>> columns, int total_columns){
    for(int row_index_ = 0; row_index_ < data_rows_count_; row_index_++){
        std::vector<std::string> row;
        for(int column_index_ = 0; column_index_ < total_columns; column_index_++) {
            row.push_back(columns[column_index_][row_index_]);
        }
        output_csv->writeRowDecoder(row);
    }
}

void DecoderCommon::closeFiles(){
    delete input_file;
    delete output_csv;
}

std::string DecoderCommon::getDatasetName(){
    return dataset->dataset_name;
}
