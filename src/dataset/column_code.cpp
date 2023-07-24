
#include "column_code.h"
#include "assert.h"
#include "math_utils.h"

ColumnCode::ColumnCode(Range* range_, int column_index) {
    updateRange(range_, column_index);
    total_bits = 0;
    total_mask_bits = 0;
}

void ColumnCode::updateRange(Range* range_, int column_index){
    range = range_;
    offset = -range->begin;

    if (column_index == 0){ // time delta column
        assert(range->begin == 0);
        bits = MathUtils::bitLength(MathUtils::intAbsolute(range->end));
        nan = -1; // should not be used since there aren't nodata entries in the timestamp delta column
    }
    else {
        nan = offset + range->end + 1;
        bits = MathUtils::bitLength(MathUtils::intAbsolute(nan));
    }
}

void ColumnCode::addBits(int bits_, bool mask_mode){
    if (mask_mode) {
        total_mask_bits += bits_;
    }
    else {
        total_bits += bits_;
    }
}

void ColumnCode::print(){
    range->print();
    std::cout << "bits = " << bits << std::endl;
    std::cout << "offset = " << offset << std::endl;
    std::cout << "nan = " << nan << std::endl;
}
