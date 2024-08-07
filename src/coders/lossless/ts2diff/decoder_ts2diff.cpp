
#include "decoder_ts2diff.h"

#include "conversor.h"
#include "math_utils.h"

std::vector<std::string> DecoderTS2Diff::decodeDataColumn(bool mask_mode_){
    std::vector<std::string> column;
    row_index = 0;
    int unprocessed_rows = data_rows_count;
    read_first_value = false;
    if (!mask_mode){
        no_data_value = dataset->column_code-> range->end + 1;
    }

#if CHECKS
    assert(mask->total_no_data + mask->total_data == data_rows_count);
#endif
    while (unprocessed_rows > 0) {
        if (mask_mode && mask->isNoData()) {
            column.push_back(Constants::NO_DATA);
            row_index++; unprocessed_rows--;
            continue;
        }

        if (!read_first_value){
            read_first_value = true;
            std::string csv_value = decodeValueRaw();
            column.push_back(csv_value);
            row_index++; unprocessed_rows--;

            if (!mask_mode && Constants::isNoData(csv_value)) {
                previous_value = no_data_value;
            }
            else {
                previous_value = Conversor::stringToInt(csv_value);
            }
            if (mask_mode) { mask->total_data -= 1; }
            continue;
        }
        int w_size = window_size;
        if (mask_mode){
            if (mask->total_data < w_size) {
                w_size = mask->total_data;
            }
        }
        else if (unprocessed_rows < w_size) {
             w_size = unprocessed_rows;
        }

        decodeWindow(this, false, mask_mode, column, w_size);
        unprocessed_rows = data_rows_count - row_index;
    }
    return column;
}

void DecoderTS2Diff::decodeWindow(DecoderTS2Diff* decoder, bool time_delta_column, bool mask_mode, std::vector<std::string> & column, int window_size){
    int min = decodeWindowMin(decoder);
    int significant_bits = decodeWindowSignificantBits(decoder);
    int value = decoder->previous_value;

    int i = 0;
    while (i < window_size){
        if (!time_delta_column && i > 0 && mask_mode && decoder->mask->isNoData()) { // always false in the first iteration
            column.push_back(Constants::NO_DATA);
            decoder->row_index++;
            continue;
        }

        if (significant_bits > 0){
            int delta = decoder->decodeInt(significant_bits);
            if (!time_delta_column) {
                value += delta;
            }
            else {
                value = delta;
            }
        }
        value += min;

        std::string value_string = Conversor::intToString(value);
        if (!time_delta_column && !mask_mode && value == decoder->no_data_value){
            value_string = Constants::NO_DATA;
        }
        column.push_back(value_string);

        decoder->previous_value = value;
        decoder->row_index++;
        i++;
    }
    if (mask_mode){
        decoder->mask->total_data -= window_size;
    }
}

int DecoderTS2Diff::decodeWindowMin(DecoderTS2Diff* decoder){
    bool negative_sign = decoder->decodeBool();
    int min = decoder->decodeInt(decoder->dataset->bits());
    if (negative_sign) { min = -min; }
    return min;
}

int DecoderTS2Diff::decodeWindowSignificantBits(DecoderTS2Diff* decoder){
    int bits_needed_to_code_significant_bits = MathUtils::bitLength(decoder->dataset->bits());
    int significant_bits = decoder->decodeInt(bits_needed_to_code_significant_bits);
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
        decodeWindow(decoder, true, false, column, w_size);
    }
    return column;
}