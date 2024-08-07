
#include "decoder_cols.h"

#include "vector_utils.h"
#include "assert.h"
#include "string_utils.h"
#include "arithmetic_mask_decoder.h"
#include "time_delta_decoder.h"

void DecoderCols::decodeDataRows(bool mask_mode_){
    mask_mode = mask_mode_;
    int total_columns = dataset->data_columns_count + 1;

    std::vector<std::vector<std::string>> columns;
    std::vector<std::string> column;

    column_index = 0;
    column = decodeColumn();
    columns.push_back(column);

    if (mask_mode){
        ArithmeticMaskDecoder* amd = new ArithmeticMaskDecoder(this, dataset->data_columns_count);
        masks_vector = amd->decode();
    }

    for(column_index = 1; column_index < total_columns; column_index++) {
        std::vector<std::string> column = decodeColumn();
        columns.push_back(column);
    }
    transposeMatrix(data_rows_count, columns, total_columns);
}

std::vector<std::string> DecoderCols::decodeColumn(){
    dataset->setColumn(column_index);
    if (column_index == 0) {
        std::vector<std::string> vec = TimeDeltaDecoder::decode(this);
        return vec;
    }
    if (mask_mode) {
        mask = masks_vector.at(column_index - 1);
    }
    std::vector<std::string> col = decodeDataColumn(mask_mode);
    return col;
}
