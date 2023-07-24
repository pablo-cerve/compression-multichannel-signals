
#include "coder_input.h"
#include "constants.h"
#include <iostream>

CoderInput::CoderInput(CoderCommon* coder_, int data_columns_count_){
    coder = coder_;
    input_csv = coder->input_csv;
    dataset = coder->dataset;
    data_columns_count = data_columns_count_;
    reset_model = false;
    eof = false;
    setNextColumn(1);
}

void CoderInput::setNextColumn(int col_index){
    column_index = col_index;
#if COUT
    std::cout << "ccode mask column_index " << column_index << std::endl;
#endif
    dataset->setColumn(column_index);
    dataset->setMode("MASK");
    input_csv->goToFirstDataRow(column_index);
    total_data_rows = 0;
    total_rows = 0;
}

void CoderInput::endCurrentColumn(){
    total_data_rows_vector.push_back(total_data_rows);
    if (column_index < data_columns_count){
        setNextColumn(column_index + 1);
        reset_model = true;
    }
    else { // column_index == data_columns_count
        eof = true;
    }
}

//
// PRE: !eof()
//
int CoderInput::getByte(){
    reset_model = false;
    int value = 1; // nodata
    std::string csv_value = input_csv->readNextValue();
    if (!Constants::isNoData(csv_value)){
        total_data_rows++;
        value = 0;
    }
    total_rows++;

    if (!input_csv->continue_reading){ // reached end of column
        endCurrentColumn();
    }
    return value;
}
