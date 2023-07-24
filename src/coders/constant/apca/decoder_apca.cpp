
#include "decoder_apca.h"
#include "assert.h"
#include "math_utils.h"

std::vector<std::string> DecoderAPCA::decodeDataColumn(bool mask_mode){
    return decodeDataColumn(this, mask_mode);
}

//
// This needs to be a static method because it is also called by DecoderGAMPS
//
std::vector<std::string> DecoderAPCA::decodeDataColumn(DecoderCommon* decoder, bool mask_mode){
    std::vector<std::string> column;
    decoder->row_index = 0;

if (mask_mode) {
#if CHECKS
    assert(decoder->mask->total_no_data + decoder->mask->total_data == decoder->data_rows_count);
#endif // END CHECKS
}

    int unprocessed_rows = decoder->data_rows_count;
    while (unprocessed_rows > 0) {
    if (mask_mode) {
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
    DecoderPCA::decodeConstantWindow(decoder, column, window_size, mask_mode);
if (mask_mode) {
    decoder->mask->total_data -= window_size;
}
}
