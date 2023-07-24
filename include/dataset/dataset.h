
#ifndef CPP_PROJECT_DATASET_H
#define CPP_PROJECT_DATASET_H

#include "column_code.h"

struct Dataset {

private:
    int array_index;
    bool header_mode;
    bool mask_mode;

public:
    ColumnCode* column_code;
    int data_columns_count; // number of columns (excluding time delta)
    std::vector<ColumnCode*> column_code_vector;
    int total_bits;
    int header_bits;
    std::string dataset_name; // included in DatasetUtils::DATASET_NAMES

    Dataset();
    void setHeaderValues(std::vector<Range*> ranges, int data_columns_count_);
    void setDatasetName(std::string dataset_name_);
    void updateRangesGAMPS(int column_index);
    void setColumn(int column_index);
    void setMode(std::string mode);
    void addBits(int bits);
    int getBits();
    int bits();
    int offset();
    int nan();
    bool insideRange(int value);
    int dataColumnsGroupCount();
    void printBits();
    std::vector<int> totalMaskBitsArray();
    std::vector<int> totalBitsArray();
//    void printRange();
};

#endif //CPP_PROJECT_DATASET_H
