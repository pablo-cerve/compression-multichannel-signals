
#ifndef CPP_PROJECT_COLUMN_CODE_H
#define CPP_PROJECT_COLUMN_CODE_H

#include <vector>
#include "range.h"

class ColumnCode {

public:
    Range* range;
    int bits;
    int offset;
    int nan;

    int total_bits;
    int total_mask_bits;

    ColumnCode(Range* range_, int column_index);
    void updateRange(Range* range_, int column_index);
    void addBits(int bits_, bool mask_mode);
    void print();
};

#endif //CPP_PROJECT_COLUMN_CODE_H
