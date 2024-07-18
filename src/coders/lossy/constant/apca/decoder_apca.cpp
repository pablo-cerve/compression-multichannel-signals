
#include "decoder_apca.h"
#include "assert.h"
#include "math_utils.h"
#include "conversor.h"

std::vector<std::string> DecoderAPCA::decodeDataColumn(bool mask_mode){
    return decodeDataColumn(this, mask_mode, 0);
}

//
// This needs to be a static method because it is also called by DecoderGAMPS
//
std::vector<std::string> DecoderAPCA::decodeDataColumn(DecoderCommon* decoder, bool mask_mode, int begin_row_index){
#if CHECKS
    if (mask_mode) {
        assert(decoder->mask->total_no_data + decoder->mask->total_data == decoder->data_rows_count);
    }
#endif
    std::vector<std::string> column;
    decoder->row_index = begin_row_index;
    int unprocessed_rows = decoder->data_rows_count;
    while (unprocessed_rows > 0) {
        if (mask_mode){
            if (decoder->mask->isNoData()) {
                column.push_back(Constants::NO_DATA);
                decoder->row_index++;
                unprocessed_rows--;
                continue;
            }
        }
        decodeWindow(decoder, column, mask_mode);
        unprocessed_rows = decoder->data_rows_count - decoder->row_index;
    }
    return column;
}

void DecoderAPCA::decodeWindow(DecoderCommon* decoder, std::vector<std::string> & column, bool mask_mode){
    int window_size = decoder->decodeWindowLength();
    decodeConstantWindow(decoder, column, window_size, mask_mode);
    if (mask_mode) {
        decoder->mask->total_data -= window_size;
    }
}

void DecoderAPCA::decodeConstantWindow(DecoderCommon* decoder, std::vector<std::string> & column, int window_size, bool mask_mode){
    std::string constant = decoder->decodeValueRaw();
    int i = 0;
    while (i < window_size){
        if (mask_mode) {
            if (i > 0 && decoder->mask->isNoData()) { // always false in the first iteration
                column.push_back(Constants::NO_DATA);
                decoder->row_index++;
                continue;
            }
        }
        column.push_back(constant);
        i++;
        decoder->row_index++;
    }
}

std::vector<std::string> DecoderAPCA::decodeTimeDelta(DecoderCommon* decoder){
    bool mask_mode = false;
    std::vector<std::string> column = DecoderAPCA::decodeDataColumn(decoder, mask_mode, 1);
    column.insert(column.begin(), "0"); // add "0" in the beginning

    for (int i=0; i < column.size(); i++){
        std::string value = column.at(i);
        int value_int = Conversor::stringToInt(value);
        decoder->time_delta_vector.push_back(value_int);
    }
    return column;
}
