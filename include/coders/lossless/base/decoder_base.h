
#ifndef CPP_PROJECT_DECODER_BASE_H
#define CPP_PROJECT_DECODER_BASE_H

#include "decoder_cols.h"

class DecoderBase: public DecoderCols {

public:
    using DecoderCols::DecoderCols;
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    static std::vector<std::string> decodeTimeDelta(DecoderBase* decoder);

private:
    std::vector<std::string> decodeDataColumnAux(bool mask_mode, int begin_row_index);

};

#endif //CPP_PROJECT_DECODER_BASE_H
