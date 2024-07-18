
#ifndef CPP_PROJECT_DECODER_CA_H
#define CPP_PROJECT_DECODER_CA_H

#include "decoder_cols.h"

class DecoderCA: public DecoderCols {

private:
    std::string archived_value;
    bool decode_archived_value;

    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    void decode(int nodata_sum, int current_time_delta);
    bool decodeArchivedValue();
    void decodeWindow(int nodata_sum);
    void decodeValues(int window_size, std::string value, int nodata_sum);

public:
    using DecoderCols::DecoderCols;
};

#endif //CPP_PROJECT_DECODER_CA_H
