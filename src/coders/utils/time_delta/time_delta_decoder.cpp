
#include "time_delta_decoder.h"
#include "conversor.h"
#include "decoder_apca.h"
#include "decoder_base.h"
#include "decoder_gorilla.h"
#include "decoder_ts2diff.h"
#include "time_delta_coder.h"

std::vector<std::string> TimeDeltaDecoder::decode(DecoderCommon* decoder){
    std::vector<std::string> column;

    if (decoder->isDecoder("Base")) {
        column = DecoderBase::decodeTimeDelta((DecoderBase*) decoder);
    }
    else if (decoder->isDecoder("Gorilla")){
        column = DecoderGorilla::decodeTimeDelta((DecoderGorilla*) decoder);
    }
    else if (decoder->isDecoder("TS2Diff")){
        column = DecoderTS2Diff::decodeTimeDelta((DecoderTS2Diff*) decoder);
    }
    else if (decoder->isDecoder("PCA*")){
        column = DecoderPCA::decodeTimeDelta((DecoderPCA*) decoder);
    }
    else {
        // save original window size param
        int window_size_bit_length = decoder->window_size_bit_length;

        int window_size_for_dataset = TimeDeltaCoder::getWindowSize(decoder->getDatasetName());
        int window_size_bit_length_for_dataset = MathUtils::windowSizeBitLength(window_size_for_dataset);
        decoder->setWindowSizeBitLength(window_size_bit_length_for_dataset);
        column = DecoderAPCA::decodeTimeDelta(decoder);

        // set original window size param
        decoder->setWindowSizeBitLength(window_size_bit_length);
    }
    return column;
}
