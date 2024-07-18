
#ifndef CPP_PROJECT_CODER_COLS_H
#define CPP_PROJECT_CODER_COLS_H

#include "coder_common.h"
#include "dataset.h"

class CoderCols: public CoderCommon {

private:
    void codeDataRows(bool is_lossless_) override;

    void codeColumn();

    virtual void codeColumnBefore() = 0;
    virtual void codeColumnWhile(std::string csv_value) = 0;
    virtual void codeColumnAfter() = 0;

protected:
    bool is_lossless = false;
    int column_index = 0;
    int row_index = 0;
    int delta_sum = 0;
    std::vector<int> time_delta_vector;
    int total_data_rows;
    std::vector<int> total_data_rows_vector;

    void codeDataColumn();
    int getEpsilon(std::vector<int> eps_vector, int col_index);

public:
    using CoderCommon::CoderCommon;
};

#endif //CPP_PROJECT_CODER_COLS_H
