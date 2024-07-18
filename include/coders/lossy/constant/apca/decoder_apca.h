
#ifndef CPP_PROJECT_DECODER_APCA_H
#define CPP_PROJECT_DECODER_APCA_H

#include "decoder_pca.h"

class DecoderAPCA: public DecoderPCA {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;

public:
    using DecoderPCA::DecoderPCA;
    static std::vector<std::string> decodeDataColumn(DecoderCommon* decoder, bool mask_mode, int begin_row_index);
    static void decodeWindow(DecoderCommon* decoder, std::vector<std::string> & column, bool mask_mode);
    static void decodeConstantWindow(DecoderCommon* decoder, std::vector<std::string> & column, int window_size, bool mask_mode);
    static std::vector<std::string> decodeTimeDelta(DecoderCommon* decoder);
};

#endif //CPP_PROJECT_DECODER_APCA_H
