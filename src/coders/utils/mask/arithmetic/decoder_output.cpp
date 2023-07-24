
#include "decoder_output.h"
#include "constants.h"

DecoderOutput::DecoderOutput(int data_columns_count_, int data_rows_count_){
    data_columns_count = data_columns_count_;
    data_rows_count = data_rows_count_;
    reset_model = false;
    eof = false;
    setNextColumn(1);
}

void DecoderOutput::setNextColumn(int col_index){
    column_index = col_index;
#if COUT
    std::cout << "decode mask column_index " << column_index << std::endl;
#endif
    row_index = 0;
    mask = new Mask();
}

void DecoderOutput::endCurrentColumn(){
    mask->close();
    mask->reset();
    // mask->print();
    assert(mask->total_data + mask->total_no_data == data_rows_count);
    masks_vector.push_back(mask);
    if (column_index < data_columns_count){
        setNextColumn(column_index + 1);
        reset_model = true;
    }
    else { // column_index == data_columns_count
        assert(masks_vector.size() == data_columns_count);
        eof = true;
    }
}

//
// PRE: !eof()
//
void DecoderOutput::putByte(int c){
    reset_model = false;
    bool nodata = c == 1;
    mask->add(nodata);
    // std::cout << "[" << row_index << "] >>>>>>>>>>>>>>>>>>> DecoderOutput = " << c << std::endl;
    row_index++;
    if (row_index == data_rows_count){
        endCurrentColumn();
    }
}
