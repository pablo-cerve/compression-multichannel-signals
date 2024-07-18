
#include "decoder_ts2diff.h"

#include "conversor.h"
#include "math_utils.h"

std::vector<std::string> DecoderTS2Diff::decodeDataColumn(bool mask_mode){
    std::vector<std::string> column;
    row_index = 0;
    int unprocessed_rows = data_rows_count;
    read_first_value = false;
    no_data_value = dataset->column_code-> range->end + 1;

#if CHECKS
    assert(mask->total_no_data + mask->total_data == data_rows_count);
#endif
    // std::cout << "column_index = " << column_index << std::endl;
    while (unprocessed_rows > 0) {
        if (!read_first_value){
            read_first_value = true; 
            std::string csv_value = decodeValueRaw();
            if (column_index == -1) std::cout << "csv_value = " << csv_value << std::endl;
            column.push_back(csv_value);
            row_index++; unprocessed_rows--;

            if (Constants::isNoData(csv_value)) { 
                previous_value = no_data_value;
            }
            else {
                previous_value = Conversor::stringToInt(csv_value);
            }
            continue;
        }
        int w_size = window_size;
        if (unprocessed_rows < w_size) { w_size = unprocessed_rows; }
        decodeWindow(this, false, column, w_size);
        unprocessed_rows = data_rows_count - row_index;
    }
    return column;
}

void DecoderTS2Diff::decodeWindow(DecoderTS2Diff* decoder, bool time_delta_column, std::vector<std::string> & column, int window_size){
    if (decoder->column_index == -1) std::cout << "********************* codeWindow" << std::endl;
    int min = decodeWindowMin(decoder);
    int significant_bits = decodeWindowSignificantBits(decoder);
    int value = decoder->previous_value;

    int i = 0;
    while (i < window_size){
        if (significant_bits > 0){
            int delta = decoder->decodeInt(significant_bits);
            if (decoder->column_index == -1) std::cout << "1coder->codeInt(" << delta << ", " << significant_bits << ");" << std::endl;
            if (!time_delta_column) {
                value += delta;
            }
            else {
                value = delta;
            }    
        }
        value += min;
        
        std::string value_string = Conversor::intToString(value);
        if (!time_delta_column && value == decoder->no_data_value){
            value_string = Constants::NO_DATA;
        }
        if (decoder->column_index == -1) std::cout << "csv_value = " << value_string << std::endl;
        column.push_back(value_string);
        decoder->previous_value = value;
        decoder->row_index++;
        i++;
    }
}

int DecoderTS2Diff::decodeWindowMin(DecoderTS2Diff* decoder){
    bool negative_sign = decoder->decodeBool();
    if (negative_sign){
        if (decoder->column_index == -1) std::cout << "coder->codeBit(1);" << std::endl;
    }
    else{
        if (decoder->column_index == -1) std::cout << "coder->codeBit(0);" << std::endl;
    }
    int min = decoder->decodeInt(decoder->dataset->bits());
    if (decoder->column_index == -1) std::cout << "2coder->codeInt(" << min << ", " << decoder->dataset->bits() << ");" << std::endl;
    if (negative_sign) { min = -min; }
    return min;
}

int DecoderTS2Diff::decodeWindowSignificantBits(DecoderTS2Diff* decoder){
    int bits_needed_to_code_significant_bits = MathUtils::bitLength(decoder->dataset->bits());
    int significant_bits = decoder->decodeInt(bits_needed_to_code_significant_bits);
    if (decoder->column_index == -1) std::cout << "3coder->codeInt(" << significant_bits << ", " << bits_needed_to_code_significant_bits << ");" << std::endl;
    return significant_bits;
}

std::vector<std::string> DecoderTS2Diff::decodeTimeDelta(DecoderTS2Diff* decoder){
    std::vector<std::string> column;
    std::string value;
    while (decoder->row_index < decoder->data_rows_count) {
        if (decoder->row_index == 0){
            column.push_back("0"); // add "0" in the beginning
            decoder->previous_value = 0;
            decoder->row_index++;
            continue;
        }
        int w_size = decoder->window_size;
        int remaining_data = decoder->data_rows_count - decoder->row_index;

        if (remaining_data < w_size) { w_size = remaining_data; }
        decodeWindow(decoder, true, column, w_size);
    }
    return column;
}