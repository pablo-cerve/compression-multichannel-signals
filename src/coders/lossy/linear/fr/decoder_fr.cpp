
#include "decoder_fr.h"
#include "coder_utils.h"
#include "assert.h"
#include "DataItem.h"
#include "conversor.h"
#include "Line.h"

std::vector<std::string> DecoderFR::decodeDataColumn(bool mask_mode_){
    column = new Column(data_rows_count, mask->total_data, mask->total_no_data);
    std::vector<int> x_coords_vector = CoderUtils::createXCoordsVectorMaskMode(mask, time_delta_vector, 0);

    mask->reset();
    while (column->notFinished()){
        if (mask->isNoData()) {
            column->addNoData();
            continue;
        }
        int remaining_data = column->unprocessed_data_rows;
        int w_size = (remaining_data < window_size) ? remaining_data : window_size;
        decodeWindow(w_size, x_coords_vector);
    }

    column->assertAfter();
    return column->column_vector;
}

void DecoderFR::decodeWindow(int window_size, std::vector<int> x_coords){
    std::vector<DataItem> data_items = readDataItems(window_size);
    int size = data_items.size();
    assert(0 < size <= window_size);
    assert(data_items[0].timestamp == 0);
    assert(data_items[size-1].timestamp == window_size - 1);

    int value;
    Line* line;
    Point *first_point, *last_point;
    std::string push_value;

    int i = 0;
    int data_item_index = 0;
    int next_timestamp = 0;
    int x_coords_index_offset = column->processed_data_rows;
    int x_coords_offset = x_coords.at(x_coords_index_offset);

    while (i < window_size) {
        if (i > 0 && mask->isNoData()) {
            // mask->isNoData() is always false in the first iteration
            column->addNoData();
            continue;
        }
        if (i == next_timestamp) {
            DataItem data_item = data_items[data_item_index];
            value = data_item.value;
            if (data_item.timestamp != window_size - 1) { // not the last data_item
                DataItem new_data_item = data_items[data_item_index + 1];
                if (new_data_item.timestamp != data_item.timestamp + 1) {
                    int first_point_x_coord = x_coords.at(x_coords_index_offset + data_item.timestamp) - x_coords_offset;
                    int last_point_x_coord = x_coords.at(x_coords_index_offset + new_data_item.timestamp) - x_coords_offset;
                    // we need to create a line because we will need to do a projection
                    // since there are points in between which were not decoded in data_items
                    first_point = new Point(data_item.value, first_point_x_coord);
                    last_point = new Point(new_data_item.value, last_point_x_coord);
                    line = new Line(first_point, last_point);
                }
                next_timestamp = new_data_item.timestamp;
                data_item_index++;
            }
        }
        else {
            // project x_coord into the line
            double double_val = line->getValue(x_coords.at(x_coords_index_offset + i) - x_coords_offset);
            value = Conversor::doubleToInt(double_val);
        }
        push_value = Conversor::intToString(value);
        column->addData(push_value);
        i++;
    }
}

std::vector<DataItem> DecoderFR::readDataItems(int window_size){
    assert(window_size > 0);
    std::vector<DataItem> window;

    bool first_index = true;
    int index;
    while(true) {
        std::string value_str = decodeValueRaw();
        int value = Conversor::stringToInt(value_str);

        if (first_index) {
            // we always code the value in the first index, so we don't have to code its index
            index = 0;
            first_index = false;
        }
        else {
            index = decodeWindowLength(); // 1 <= index <= window_size
        }
        window.push_back(DataItem(value, index));
        if (index == window_size - 1) { break; }
    }
    return window;
}
