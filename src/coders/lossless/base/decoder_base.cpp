
#include "decoder_base.h"
#include "assert.h"
#include "vector_utils.h"

std::vector<std::string> DecoderBase::decodeDataColumn(bool mask_mode_){
    return decodeDataColumnAux(0);
}

std::vector<std::string> DecoderBase::decodeDataColumnAux(int begin_row_index){
    std::vector<std::string> column;
    row_index = begin_row_index;

    while (row_index < data_rows_count) {
        std::string value = decodeValueRaw();
        column.push_back(value);
        row_index++;
    }
    return column;
}

std::vector<std::string> DecoderBase::decodeTimeDelta(DecoderBase* decoder){
    std::vector<std::string> column = decoder->decodeDataColumnAux(1);
    column.insert(column.begin(), "0"); // add "0" in the beginning

    for (int i=0; i < column.size(); i++){
        std::string value = column.at(i);
        int value_int = Conversor::stringToInt(value);
        decoder->time_delta_vector.push_back(value_int);
    }
    return column;
}