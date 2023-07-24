
#include "dataset.h"
#include "assert.h"
#include "constants.h"

Dataset::Dataset(){
    total_bits = 0;
    header_bits = 0;
    header_mode = true;
}

void Dataset::setHeaderValues(std::vector<Range*> ranges, int data_columns_count_){
    assert(ranges.size() > 0);
    assert(ranges.size() <= data_columns_count_ + 1);

    for(int i=0; i < ranges.size(); i++){
        ColumnCode* new_column_code = new ColumnCode(ranges[i], i);
        column_code_vector.push_back(new_column_code);
    }
    data_columns_count = data_columns_count_;
    array_index = 0;
    column_code = column_code_vector[array_index];
}

void Dataset::setDatasetName(std::string dataset_name_){
    dataset_name = dataset_name_;
}

void Dataset::updateRangesGAMPS(int base_column_index){
#if CHECKS
    assert(base_column_index > 0); // cannot be the delta time column
#endif
    ColumnCode* current_column_code = column_code_vector.at(base_column_index);
    int nan_minus_one = current_column_code->nan - 1;
    Range* range = new Range(-nan_minus_one, nan_minus_one);
    current_column_code->updateRange(range, base_column_index);
}

void Dataset::setColumn(int column_index){
    array_index = 0;
    if (column_index != 0){
        array_index = column_index % (column_code_vector.size() - 1);
        if (array_index == 0){ array_index = column_code_vector.size() - 1; }
    }
    column_code = column_code_vector[array_index];
}

void Dataset::setMode(std::string mode){
    if (mode.compare("DATA") == 0){
        mask_mode = false;
        header_mode = false;
    }
    else if (mode.compare("MASK") == 0){
        mask_mode = true;
        header_mode = false;
    }
    else if (mode.compare("HEADER") == 0){
        header_mode = true; // mask_mode doesn't matter
    }
    else {
        throw std::invalid_argument(mode);
    }
}

void Dataset::addBits(int bits){
    total_bits += bits;
    if (header_mode){
        header_bits += bits;
    }
    else {
        column_code->addBits(bits, mask_mode);
    }
}

int Dataset::getBits(){
    addBits(column_code->bits);
    return column_code->bits;
}

int Dataset::bits(){
    return column_code->bits;
}

int Dataset::offset(){
    return column_code->offset;
}

int Dataset::nan(){
    return column_code->nan;
}

bool Dataset::insideRange(int value){
    return column_code->range->insideRange(value);
}

int Dataset::dataColumnsGroupCount(){
    return column_code_vector.size() - 1; // -1 because of the time delta column
}

void Dataset::printBits(){
    int total = header_bits;
    std::cout << "header_bits " << header_bits << std::endl;
    for(int i=0; i<column_code_vector.size(); i++){
        int total_mask_bits = column_code_vector[i]->total_mask_bits;
        int total_bits = column_code_vector[i]->total_bits;
        std::cout << "total_mask_bits " << total_mask_bits << std::endl;
        std::cout << "total_bits " << total_bits << std::endl;
        total += total_mask_bits + total_bits;
    }
    std::cout << "SUM = " << total << std::endl;
}

std::vector<int> Dataset::totalMaskBitsArray(){
    int size = column_code_vector.size();
    std::vector<int> res(size);
    for(int i=0; i < size; i++){
        res[i] = column_code_vector[i]->total_mask_bits;
    }
    return res;
}

std::vector<int> Dataset::totalBitsArray(){
    int size = column_code_vector.size();
    std::vector<int> res(size);
    for(int i=0; i < size; i++){
        res[i] = column_code_vector[i]->total_bits;
    }
    return res;
}

//void Dataset::printRange(){
//    column_code.range.print();
//}