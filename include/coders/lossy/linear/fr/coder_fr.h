
#ifndef CPP_PROJECT_CODER_FR_H
#define CPP_PROJECT_CODER_FR_H

#include "coder_cols.h"
#include "fr_window.h"

class CoderFR: public CoderCols {

private:
    int window_size_bit_length;
    std::vector<int> epsilons_vector;
    FRWindow* window;

    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

    FRWindow* createWindow();
    void codeWindow();
    void codeItem(DataItem item, int index);

public:
    using CoderCols::CoderCols;
    void setCoderParams(int window_size_, std::vector<int> epsilons_vector_);

};

#endif //CPP_PROJECT_CODER_FR_H
