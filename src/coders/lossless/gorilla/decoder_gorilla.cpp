
#include "decoder_gorilla.h"

#include "gorilla_constants.h"
#include "conversor.h"

std::vector<std::string> DecoderGorilla::decodeDataColumn(bool mask_mode){
    std::vector<std::string> column;
    row_index = 0;
    read_first_value = false;
    no_data_value = dataset->column_code-> range->end + 1;

    int unprocessed_rows = data_rows_count;
#if CHECKS
    assert(mask->total_no_data + mask->total_data == data_rows_count);
#endif

    while (unprocessed_rows > 0) {
        double value;
        if (!read_first_value){
            read_first_value = true;    
            std::string csv_value = decodeValueRaw();
            column.push_back(csv_value);
            row_index++; unprocessed_rows--;

            if (Constants::isNoData(csv_value)) { 
                value = no_data_value;
            }
            else{
                value = Conversor::stringToDouble(csv_value);
            }
            value += dataset->offset();
            assert(value >= 0);
            previousValue = (uint64_t) value;
            previousLeadingZeros = __builtin_clzll(previousValue);
            previousTrailingZeros = __builtin_ctzll(previousValue);
            continue;
        }

        value = decodeNextValue();
        int int_value = value - dataset->offset();
        std::string push_value;
        if (int_value == no_data_value){
            push_value = Constants::NO_DATA;
        }
        else{
            push_value = Conversor::intToString(int_value);
        }
        column.push_back(push_value);
        row_index++; unprocessed_rows--;
    }
    return column;
}


double DecoderGorilla::decodeNextValue(){
    // The following code is based on 
    // https://github.com/facebookarchive/beringei/blob/75c3002b179d99c8709323d605e7d4b53484035c/beringei/lib/TimeSeriesStream.cpp#L282
    //
    uint64_t nonZeroValue = decodeBool();

    if (!nonZeroValue) {
        double p = (double) previousValue;
        return p;
    }

    uint64_t usePreviousBlockInformation = decodeBool();

    uint64_t xorValue;
    if (usePreviousBlockInformation) {
        xorValue = decodeInt(GorillaConstants::kValue - previousLeadingZeros - previousTrailingZeros);
        xorValue <<= previousTrailingZeros;
    } else {
        uint64_t leadingZeros = decodeInt(GorillaConstants::kLeadingZerosLengthBits);
        uint64_t blockSize = decodeInt(GorillaConstants::kBlockSizeLengthBits) + GorillaConstants::kBlockSizeAdjustment;
        
        previousTrailingZeros = GorillaConstants::kValue - blockSize - leadingZeros;
        xorValue = decodeInt(blockSize);

        xorValue <<= previousTrailingZeros;
        previousLeadingZeros = leadingZeros;
    }

    uint64_t value = xorValue ^ previousValue;
    previousValue = value;

    double p = (double) value;
    return p;
}

std::vector<std::string> DecoderGorilla::decodeTimeDelta(DecoderGorilla* decoder){
    std::vector<std::string> column;
    int previous_delta = 0; // t_(n−1) − t_(n−2)
    int current_delta = 0; // t_n − t_(n−1)
    int row_index = 0;

    while (row_index < decoder->data_rows_count) {
        if (row_index == 0){
            column.push_back("0"); // add "0" in the beginning
            row_index++;
            continue;
        }
        int d = decodeD(decoder);
        current_delta = d + previous_delta;
        previous_delta = current_delta;
        std::string value = Conversor::intToString(current_delta);
        column.push_back(value);
        row_index++;
    }
    return column;
}

int DecoderGorilla::decodeD(DecoderGorilla* decoder){
    // The following timestamps are encoded as follows:
    // (a) Calculate the delta of delta
    //       D = (t_n − t_(n−1)) − (t_(n−1) − t_(n−2))
    bool bit = decoder->decodeBool();
    int value;

    if (!bit){
        // (b) If D is zero, then store a single ‘0’ bit
        return 0;
    }

    bit = decoder->decodeBool();
    if (!bit){
        // (c) If D is between [-63, 64], store ‘10’ followed by the value (7 bits)
        value = decoder->decodeInt(7);
        value -= 63;
        return value;
    }

    bit = decoder->decodeBool();
    if (!bit){
        // (d) If D is between [-255, 256], store ‘110’ followed by the value (9 bits)
        value = decoder->decodeInt(9);
        value -= 255;
        return value;
    }

    bit = decoder->decodeBool();
    if (!bit){
        // (e) if D is between [-2047, 2048], store ‘1110’ followed by the value (12 bits)
        value = decoder->decodeInt(12);
        value -= 2047;
        return value;
    }

    // (f) Otherwise store ‘1111’ followed by D using 32 bits
    value = decoder->decodeInt(32);
    value -= 2147483647;
    return value;
}
