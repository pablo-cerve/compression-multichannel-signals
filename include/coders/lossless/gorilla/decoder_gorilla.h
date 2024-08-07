
#ifndef CPP_PROJECT_DECODER_GORILLA_H
#define CPP_PROJECT_DECODER_GORILLA_H

#include "decoder_cols.h"

class DecoderGorilla: public DecoderCols {

public:
    using DecoderCols::DecoderCols;
    std::vector<std::string> decodeDataColumn(bool mask_mode_) override;
    static std::vector<std::string> decodeTimeDelta(DecoderGorilla* decoder);

private:
    static int decodeD(DecoderGorilla* decoder);
    double decodeNextValue();

    uint64_t previousValue;
    uint64_t previousLeadingZeros;
    uint64_t previousTrailingZeros;

    bool read_first_value;
    int no_data_value;
};

#endif //CPP_PROJECT_DECODER_GORILLA_H
