
#include "coder_cols.h"

#include "assert.h"
#include "string_utils.h"
#include "arithmetic_mask_coder.h"
#include "time_delta_coder.h"
#include "coder_utils.h"

void CoderCols::codeDataRows(bool is_lossless_){
    is_lossless = is_lossless_;
    int total_columns = dataset->data_columns_count + 1;

    column_index = 0;
    codeColumn();

    if (!is_lossless){
        ArithmeticMaskCoder* amc = new ArithmeticMaskCoder(this, dataset->data_columns_count);
        total_data_rows_vector = amc->code();
    }

    for(column_index = 1; column_index < total_columns; column_index++) {
        codeColumn();
    }
}

void CoderCols::codeColumn() {
#if COUT
    std::cout << "ccode column_index " << column_index << std::endl;
#endif
    dataset->setColumn(column_index);
    if (column_index == 0) {
        dataset->setMode("DATA");
        time_delta_vector = TimeDeltaCoder::code(this);
        return;
    }

    if (!is_lossless){
        total_data_rows = total_data_rows_vector.at(column_index - 1);
    }
    dataset->setMode("DATA");
    codeDataColumn();
}

void CoderCols::codeDataColumn(){
    this->codeColumnBefore();

    row_index = 0;
    input_csv->goToFirstDataRow(column_index);
    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        this->codeColumnWhile(csv_value);
        row_index++;
    }
    this->codeColumnAfter();
}

int CoderCols::getEpsilon(std::vector<int> eps_vector, int col_index){
    try {
        return eps_vector.at(col_index);
    }
    catch(...) {
        int vector_size = eps_vector.size();
        throw std::invalid_argument("The number of epsilons provided (" + std::to_string(vector_size) + ") is less than expected.");
    }
}
