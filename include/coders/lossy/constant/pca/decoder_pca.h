
#ifndef CPP_PROJECT_DECODER_PCA_H
#define CPP_PROJECT_DECODER_PCA_H

#include "decoder_cols.h"

class DecoderPCA: public DecoderCols {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    
public:
    using DecoderCols::DecoderCols;
    static std::vector<std::string> decodeDataColumn(DecoderPCA* decoder, bool mask_mode, int begin_row_index);
    static void decodeWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode);
    static void decodeNonConstantWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode);
    static void decodeConstantWindow(DecoderPCA* decoder, std::vector<std::string> & column, int window_size, bool mask_mode);
    static std::vector<std::string> decodeTimeDelta(DecoderPCA* decoder);
};

#endif //CPP_PROJECT_DECODER_PCA_H
