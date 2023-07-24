
#ifndef CPP_PROJECT_CODER_CA_H
#define CPP_PROJECT_CODER_CA_H

#include "coder_cols.h"
#include "ca_window.h"

class CoderCA: public CoderCols {

private:
    std::vector<int> epsilons_vector;
    CAWindow* window;

    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

    void processValue(std::string x);

    void codeArchivedValueAndCreateNonNanWindow(std::string archived_value, int archived_value_int);
    void codeWindow();
    void codeWindow(int window_length, std::string window_value);

public:
    using CoderCols::CoderCols;
    void setCoderParams(int window_size_, std::vector<int> epsilons_vector_);

};

#endif //CPP_PROJECT_CODER_CA_H
