
#include "decoder_pca.h"
#include "assert.h"
#include "conversor.h"

std::vector<std::string> DecoderPCA::decodeDataColumn(bool mask_mode){
    return decodeDataColumn(this, mask_mode, 0);
}

std::vector<std::string> DecoderPCA::decodeDataColumn(DecoderPCA* decoder, bool mask_mode, int begin_row_index){
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
        int w_size = decoder->window_size;
        if (mask_mode){
            if (decoder->mask->total_data < w_size) { w_size = decoder->mask->total_data; }
        }
        else {
            if (unprocessed_rows < w_size) { w_size = unprocessed_rows; }
        }
        decodeWindow(decoder, column, w_size, mask_mode);
        unprocessed_rows = decoder->data_rows_count - decoder->row_index;
    }
    return column;
}

void DecoderPCA::decodeWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode){
    bool fi = decoder->decodeBool();
    if (fi){ decodeNonConstantWindow(decoder, column, window_size, mask_mode); }
    else {
        decodeConstantWindow(decoder, column, window_size, mask_mode);
    }
    if (mask_mode) { decoder->mask->total_data -= window_size; }
}

void DecoderPCA::decodeConstantWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode){
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

void DecoderPCA::decodeNonConstantWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode){
    int i = 0;
    while (i < window_size){
        if (mask_mode) {
            if (i > 0 && decoder->mask->isNoData()) { // always false in the first iteration
                column.push_back(Constants::NO_DATA);
                decoder->row_index++;
                continue;
            }
        }
        std::string value = decoder->decodeValueRaw();
        column.push_back(value);
        i++;
        decoder->row_index++;
    }
}

std::vector<std::string> DecoderPCA::decodeTimeDelta(DecoderPCA* decoder){
    bool mask_mode = false;
    std::vector<std::string> column = DecoderPCA::decodeDataColumn(decoder, mask_mode, 1);
    column.insert(column.begin(), "0"); // add "0" in the beginning

    for (int i=0; i < column.size(); i++){
        std::string value = column.at(i);
        int value_int = Conversor::stringToInt(value);
        decoder->time_delta_vector.push_back(value_int);
    }
    return column;
}