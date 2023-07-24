
#ifndef CPP_PROJECT_CODER_BASE_H
#define CPP_PROJECT_CODER_BASE_H

#include "coder_cols.h"

class CoderBase: public CoderCols {

private:
    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

public:
    using CoderCols::CoderCols;
};

#endif //CPP_PROJECT_CODER_BASE_H
