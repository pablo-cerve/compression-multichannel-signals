
#ifndef CPP_PROJECT_DECODER_APCA_H
#define CPP_PROJECT_DECODER_APCA_H

#include "decoder_pca.h"

class DecoderAPCA: public DecoderPCA {

private:
    std::vector<std::string> decodeDataColumn(bool mask_mode) override;


public:
    using DecoderPCA::DecoderPCA;
    static std::vector<std::string> decodeDataColumn(DecoderCommon* decoder, bool mask_mode);
    static void decodeWindow(DecoderCommon* decoder, std::vector<std::string> & column, bool mask_mode);
};

#endif //CPP_PROJECT_DECODER_APCA_H
