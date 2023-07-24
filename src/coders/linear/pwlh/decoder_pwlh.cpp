
#include "decoder_pwlh.h"

#include "math_utils.h"
#include "pwlh_window.h"
#include <cfloat>
#include "vector_utils.h"
#include "constants.h"
#include "conversor.h"
#include "coder_utils.h"
#include "line_utils.h"
#include "linear_coder_utils.h"

void DecoderPWLH::setIntegerMode(){
    integer_mode = coder_name == "PWLH*Int";
}

std::vector<std::string> DecoderPWLH::decodeDataColumn(bool mask_mode){
    column = new Column(data_rows_count, mask->total_data, mask->total_no_data);

    while (column->notFinished()){
        if (mask->isNoData()) {
            column->addNoData();
            continue;
        }
        decodeWindow();
    }

#if CHECKS
    column->assertAfter();
#endif
    return column->column_vector;
}

void DecoderPWLH::decodeWindow(){
    int window_size = decodeWindowLength();
    integer_mode ? decodeWindowInt(window_size) : decodeWindowDouble(window_size);
}

void DecoderPWLH::decodeWindowDouble(int window_size){
    float value = decodeFloat();
    if (window_size > 1) {
        float point1_y = value;
        float point2_y = decodeFloat();
        std::vector<int> x_coords_with_nodata = LinearCoderUtils::createXCoordsVectorPWLH(this, window_size, column->row_index);
        std::vector<int> x_coords = VectorUtils::removeOccurrences(x_coords_with_nodata, -1); // remove nodata
        std::vector<std::string> decoded_points = LineUtils::decodePoints(point1_y, point2_y, x_coords);
        LinearCoderUtils::addPointsWithNoData(column, window_size, decoded_points, x_coords_with_nodata);
    }
    else {
        // window_size == 1 => this code can only run the last time decodeWindow is called
        int value_int = static_cast<int>(value);
        std::string value_str = Conversor::intToString(value_int);
        column->addData(value_str);
    }
}

void DecoderPWLH::decodeWindowInt(int window_size){
    std::string value = decodeValueRaw();
    if (window_size > 1) {
        std::string point1_y = value;
        std::string point2_y = decodeValueRaw();
        std::vector<int> x_coords_with_nodata = LinearCoderUtils::createXCoordsVectorPWLH(this, window_size, column->row_index);
        std::vector<int> x_coords = VectorUtils::removeOccurrences(x_coords_with_nodata, -1); // remove nodata
        std::vector<std::string> decoded_points = LineUtils::decodePointsString(point1_y, point2_y, x_coords);
        LinearCoderUtils::addPointsWithNoData(column, window_size, decoded_points, x_coords_with_nodata);
    }
    else {
        // window_size == 1 => this code can only run the last time decodeWindow is called
        column->addData(value);
    }
}

