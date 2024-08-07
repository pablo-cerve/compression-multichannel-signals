
#ifndef CPP_PROJECT_CODER_TS2DIFF_H
#define CPP_PROJECT_CODER_TS2DIFF_H

#include "coder_cols.h"
#include "ts2diff_window.h"

class CoderTS2Diff: public CoderCols {

public:
    using CoderCols::CoderCols;
    void setWindowSize(int window_size_);
    static void codeWindow(CoderTS2Diff* coder, TS2DiffWindow* window);
    static std::vector<int> codeTimeDelta(CoderTS2Diff* coder, int window_size);

private:
    bool read_first_value;
    int previous_value;
    int no_data_value;

    TS2DiffWindow* window;

    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;

    static void codeWindowMin(CoderTS2Diff* coder, TS2DiffWindow* window);
    static int codeWindowSignificantBits(CoderTS2Diff* coder, TS2DiffWindow* window);
};

#endif //CPP_PROJECT_CODER_TS2DIFF_H
