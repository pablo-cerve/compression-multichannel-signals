
#include "time_delta_decoder.h"
#include "conversor.h"
#include "decoder_apca.h"
#include "time_delta_coder.h"

std::vector<std::string> TimeDeltaDecoder::decode(DecoderCommon* decoder){
    int window_size_bit_length = decoder->window_size_bit_length;

    int window_size_for_dataset = TimeDeltaCoder::getWindowSize(decoder->getDatasetName());
    int window_size_bit_length_for_dataset = MathUtils::windowSizeBitLength(window_size_for_dataset);
    decoder->setWindowSizeBitLength(window_size_bit_length_for_dataset);
    bool mask_mode = false;
    std::vector<std::string> column = DecoderAPCA::decodeDataColumn(decoder, mask_mode);

    decoder->setWindowSizeBitLength(window_size_bit_length);

    for (int i=0; i < column.size(); i++){
        std::string value = column.at(i);
        int value_int = Conversor::stringToInt(value);
        decoder->time_delta_vector.push_back(value_int);
    }
    return column;
}
