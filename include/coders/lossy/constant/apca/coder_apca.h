
#ifndef CPP_PROJECT_CODER_APCA_H
#define CPP_PROJECT_CODER_APCA_H

#include "coder_cols.h"
#include "apca_window.h"

class CoderAPCA: public CoderCols {

private:
    std::vector<int> epsilons_vector;
    APCAWindow* window;
    bool mask_mode;

    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

public:
    using CoderCols::CoderCols;
    void setCoderParams(int window_size_, std::vector<int> epsilons_vector_, bool mask_mode_);
    void codeWindow(APCAWindow* window);
    static std::vector<int> codeTimeDelta(CoderCommon* coder, int window_size);
};

#endif //CPP_PROJECT_CODER_APCA_H
