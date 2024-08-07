
#include "coder_gorilla.h"
#include <assert.h>
#include "conversor.h"

void CoderGorilla::codeColumnBefore() {
    read_first_value = false;
    if (!mask_mode) {
        no_data_value = dataset->column_code-> range->end + 1;
    }
}

void CoderGorilla::codeColumnWhile(std::string csv_value){
    int value;
    if (Constants::isNoData(csv_value)) {
        if (mask_mode) { return; } // skip gap

        value = no_data_value;
    }
    else {
        value = Conversor::stringToDouble(csv_value);
    }
    value += dataset->offset();

    assert(value >= 0);

    if (!read_first_value) {
        read_first_value = true;
        codeValueRaw(csv_value);

        previousValue_ = (uint64_t) value;
        previousValueLeadingZeros_ = __builtin_clzll(previousValue_);
        previousValueTrailingZeros_ = __builtin_ctzll(previousValue_);
        return;
    }

    // The following code is based on
    // https://github.com/facebookarchive/beringei/blob/75c3002b179d99c8709323d605e7d4b53484035c/beringei/lib/TimeSeriesStream.cpp#L178
    //
    uint64_t p = (uint64_t) value;
    uint64_t xorWithPrevius = previousValue_ ^ p;

    // Doubles are encoded by XORing them with the previous value.  If
    // XORing results in a zero value (value is the same as the previous
    // value), only a single zero bit is stored, otherwise 1 bit is
    // stored. TODO : improve this with RLE for the number of zeros
    //
    // For non-zero XORred results, there are two choices:
    //
    // 1) If the block of meaningful bits falls in between the block of
    //    previous meaningful bits, i.e., there are at least as many
    //    leading zeros and as many trailing zeros as with the previous
    //    value, use that information for the block position and just
    //    store the XORred value.
    //
    // 2) Length of the number of leading zeros is stored in the next 5
    //    bits, then length of the XORred value is stored in the next 6
    //    bits and finally the XORred value is stored.

    if (xorWithPrevius == 0) { // same value
        codeBit(0);
        return;
    }

    codeBit(1);

    int leadingZeros = __builtin_clzll(xorWithPrevius);
    int trailingZeros = __builtin_ctzll(xorWithPrevius);

    if (leadingZeros > kMaxLeadingZerosLength) {
        leadingZeros = kMaxLeadingZerosLength;
    }

    int blockSize = GorillaConstants::kValue - leadingZeros - trailingZeros;
    uint32_t expectedSize = GorillaConstants::kLeadingZerosLengthBits + GorillaConstants::kBlockSizeLengthBits + blockSize;
    uint32_t previousBlockInformationSize = GorillaConstants::kValue - previousValueTrailingZeros_ - previousValueLeadingZeros_;

    if (leadingZeros >= previousValueLeadingZeros_ &&
        trailingZeros >= previousValueTrailingZeros_ &&
        previousBlockInformationSize < expectedSize) {
        // Control bit for using previous block information.
        codeBit(1);

        uint64_t blockValue = xorWithPrevius >> previousValueTrailingZeros_;
        codeInt(blockValue, previousBlockInformationSize);

    } else {
        // Control bit for not using previous block information.
        codeBit(0);

        codeInt(leadingZeros, GorillaConstants::kLeadingZerosLengthBits);
        codeInt(blockSize - GorillaConstants::kBlockSizeAdjustment, GorillaConstants::kBlockSizeLengthBits);

        uint64_t blockValue = xorWithPrevius >> trailingZeros;
        codeInt(blockValue, blockSize);

        previousValueTrailingZeros_ = trailingZeros;
        previousValueLeadingZeros_ = leadingZeros;
    }

    previousValue_ = p;
}

void CoderGorilla::codeColumnAfter() {}

std::vector<int> CoderGorilla::codeTimeDelta(CoderCols* coder){
    CSVReader* input_csv = coder->input_csv;

    std::vector<int> time_delta_vector{};
    input_csv->goToFirstDataRow(0);
    bool first_value = true;
    int previous_delta = 0; // t_(n−1) − t_(n−2)
    int current_delta = 0; // t_n − t_(n−1)

    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        int csv_value_int = Conversor::stringToInt(csv_value);
        time_delta_vector.push_back(csv_value_int);

        if (first_value){
            // the first value is always 0, no need to encode anything
            assert(csv_value == "0");
            first_value = false;
            continue;
        }

        current_delta = csv_value_int; // current_delta must be >= 0
        codeD(coder, current_delta, previous_delta);
        previous_delta = current_delta; // previous_delta must be >= 0
    }
    return time_delta_vector;
}

void CoderGorilla::codeD(CoderCols* coder, int current_delta, int previous_delta){
    // The following timestamps are encoded as follows:
    // (a) Calculate the delta of delta
    //       D = (t_n − t_(n−1)) − (t_(n−1) − t_(n−2))
    int d = current_delta - previous_delta;

    if (d == 0){
        // (b) If D is zero, then store a single ‘0’ bit
        coder->codeUnaryInv(0); // '0'
        return;
    }

    if (d >= -63 && d <= 64){
        // (c) If D is between [-63, 64], store ‘10’ followed by the value (7 bits)
        d += 63; // d in [0, 127] = [0, 2**7 - 1]
        coder->codeUnaryInv(1); // '10'
        coder->codeInt(d, 7);
        return;
    }

    if (d >= -255 && d <= 256){
        // (d) If D is between [-255, 256], store ‘110’ followed by the value (9 bits)
        d+= 255; // d in [0, 511] = [0, 2**9 - 1]
        coder->codeUnaryInv(2); // '110'
        coder->codeInt(d, 9);
        return;
    }

    if (d >= -2047 && d <= 2048){
        // (e) if D is between [-2047, 2048], store ‘1110’ followed by the value (12 bits)
        d += 2047; // d in [0, 4095] = [0, 2**12 - 1]
        coder->codeUnaryInv(3); // '1110'
        coder->codeInt(d, 12);
        return;
    }

    // (f) Otherwise store ‘1111’ followed by D using 32 bits
    // assert(d >= -2147483647 && d <= 2147483648);
    d += 2147483647; // d in [0, 4294967295] = [0, 2**32 - 1]

    coder->codeBits(1, 4); // '1111'
    coder->codeInt(d, 32);
}
