
#ifndef CPP_PROJECT_DECODER_PCA_H
#define CPP_PROJECT_DECODER_PCA_H

#include "decoder_cols.h"

class DecoderPCA: public DecoderCols {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    void decodeWindow(std::vector<std::string> & column, int window_size);
    void decodeNonConstantWindow(std::vector<std::string> & column, int window_size);

public:
    using DecoderCols::DecoderCols;
    static void decodeConstantWindow(DecoderCommon* decoder, std::vector<std::string> & column, int window_size, bool mask_mode);
};

#endif //CPP_PROJECT_DECODER_PCA_H
