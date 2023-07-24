
#ifndef CPP_PROJECT_DECODER_OUTPUT_H
#define CPP_PROJECT_DECODER_OUTPUT_H

#include "mask.h"

class DecoderOutput {

private:
    Mask* mask;
    Burst* burst;
    int data_columns_count;
    int data_rows_count;
    int column_index;
    int row_index;

    void setNextColumn(int col_index);
    void endCurrentColumn();

public:
    std::vector<Mask*> masks_vector;

    // variables used by decompressor.h
    bool reset_model;
    bool eof;

    DecoderOutput(int data_columns_count_, int data_rows_count_);
    void putByte(int c);
};

#endif //CPP_PROJECT_DECODER_OUTPUT_H
