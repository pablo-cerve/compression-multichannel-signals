
#include "decoder_ca.h"
#include "string_utils.h"
#include "math_utils.h"
#include <cmath>
#include <vector_utils.h>
#include "Line.h"
#include "coder_utils.h"
#include "line_utils.h"
#include "linear_coder_utils.h"

std::vector<std::string> DecoderCA::decodeDataColumn(bool mask_mode){
    decode_archived_value = true; // always decode an archived value before decoding a window
    column = new Column(data_rows_count, mask->total_data, mask->total_no_data);
    int nodata_sum = 0;
    while (column->notFinished()){
        int current_time_delta = time_delta_vector.at(column->row_index);
        if (mask->isNoData()) {
            nodata_sum += current_time_delta;
            column->addNoData();
            continue;
        }
        decode(nodata_sum, current_time_delta);
        nodata_sum = 0;
    }
#if CHECKS
    column->assertAfter();
#endif
    return column->column_vector;
}

void DecoderCA::decode(int nodata_sum, int current_time_delta) {
    if (decode_archived_value || (nodata_sum + current_time_delta == 0)) {
        decodeArchivedValue();
        decode_archived_value = false;
    } else {
        decodeWindow(nodata_sum);
        decode_archived_value = true;
    }
}

bool DecoderCA::decodeArchivedValue() {
    std::string value = decodeValueRaw();
    column->addData(value);
    archived_value = value;
    return false;
}

void DecoderCA::decodeWindow(int nodata_sum){
    std::string value = decodeValueRaw();
    int window_size = decodeWindowLength();
    decodeValues(window_size, value, nodata_sum);
}

void DecoderCA::decodeValues(int window_size, std::string value, int nodata_sum){
    std::vector<int> x_coords_with_nodata = LinearCoderUtils::createXCoordsVectorCA(this, window_size + 1, column->row_index-1, nodata_sum);
    std::vector<int> x_coords = VectorUtils::removeOccurrences(x_coords_with_nodata, -1); // remove nodata
    std::vector<std::string> decoded_points = LineUtils::decodePointsString(archived_value, value, x_coords);

#if CHECKS
    assert(decoded_points.at(0) == archived_value); // the first decoded point is equal to archived_value...
    assert(x_coords_with_nodata.at(0) == 0);
#endif
    decoded_points.erase(decoded_points.begin()); // ...we must remove it
    x_coords_with_nodata.erase(x_coords_with_nodata.begin());
    LinearCoderUtils::addPointsWithNoData(column, window_size, decoded_points, x_coords_with_nodata);
}
