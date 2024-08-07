
#ifndef CPP_PROJECT_DECODER_TS2DIFF_H
#define CPP_PROJECT_DECODER_TS2DIFF_H

#include "decoder_cols.h"

class DecoderTS2Diff: public DecoderCols {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode_) override;
    static void decodeWindow(DecoderTS2Diff* decoder, bool time_delta_column, bool mask_mode, std::vector<std::string> & column, int window_size);
    static int decodeWindowMin(DecoderTS2Diff* decoder);
    static int decodeWindowSignificantBits(DecoderTS2Diff* decoder);

public:
    bool read_first_value;
    int previous_value;
    int no_data_value;

    using DecoderCols::DecoderCols;
    static std::vector<std::string> decodeTimeDelta(DecoderTS2Diff* decoder);

};

#endif //CPP_PROJECT_DECODER_TS2DIFF_H
