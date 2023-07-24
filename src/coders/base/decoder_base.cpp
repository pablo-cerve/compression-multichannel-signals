
#include "decoder_base.h"
#include "assert.h"
#include "vector_utils.h"

std::vector<std::string> DecoderBase::decodeDataColumn(bool mask_mode){
    std::vector<std::string> column;
    row_index = 0;

    while (row_index < data_rows_count) {
        std::string value = decodeValueRaw();
        column.push_back(value);
        row_index++;
    }
    return column;
}
