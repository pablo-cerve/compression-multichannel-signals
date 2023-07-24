
#ifndef CPP_PROJECT_DECODER_COLS_H
#define CPP_PROJECT_DECODER_COLS_H

#include "decoder_common.h"
#include "column.h"

class DecoderCols: public DecoderCommon {

private:
    void decodeDataRows(bool base_decoder_) override;

    std::vector<std::string> decodeColumn();
    virtual std::vector<std::string> decodeDataColumn(bool mask_mode) = 0;

protected:
    bool base_decoder = false;
    int column_index;
    Column* column;

public:
    using DecoderCommon::DecoderCommon;

};

#endif //CPP_PROJECT_DECODER_COLS_H
