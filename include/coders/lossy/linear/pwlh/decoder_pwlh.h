
#ifndef CPP_PROJECT_DECODER_PWLH_H
#define CPP_PROJECT_DECODER_PWLH_H

#include "decoder_cols.h"

class DecoderPWLH: public DecoderCols {

private:
    bool integer_mode;

    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    void decodeWindow();
    void decodeWindowDouble(int window_size);
    void decodeWindowInt(int window_size);

public:
    using DecoderCols::DecoderCols;
    void setIntegerMode();
};

#endif //CPP_PROJECT_DECODER_PWLH_H
