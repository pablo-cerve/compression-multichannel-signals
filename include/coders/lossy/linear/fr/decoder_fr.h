
#ifndef CPP_PROJECT_DECODER_FR_H
#define CPP_PROJECT_DECODER_FR_H

#include "decoder_cols.h"
#include "DataItem.h"

class DecoderFR: public DecoderCols {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    void decodeWindow(int window_size, std::vector<int> x_coords);
    std::vector<DataItem> readDataItems(int window_size);

public:
    using DecoderCols::DecoderCols;
};

#endif //CPP_PROJECT_DECODER_FR_H
