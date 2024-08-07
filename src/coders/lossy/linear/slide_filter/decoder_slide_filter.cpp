
#include "decoder_slide_filter.h"
#include "vector_utils.h"
#include "math_utils.h"
#include <cmath>
#include "conversor.h"
#include "coder_utils.h"
#include "line_utils.h"

std::vector<std::string> DecoderSlideFilter::decodeDataColumn(bool mask_mode_){
    column = new Column(data_rows_count, mask->total_data, mask->total_no_data);

    if (mask->total_data > 0){
        std::vector<int> x_coords_vector = CoderUtils::createXCoordsVectorMaskModeSF(mask, time_delta_vector, 1);
        decompress(x_coords_vector);
        assert(column->unprocessed_data_rows == 0);
    }
    while (column->notFinished()) {
        column->addNoData();
    }
    column->assertAfter();
    return column->column_vector;
}

SlideFiltersEntry* DecoderSlideFilter::decodeEntry(){
    bool connToFollow = decodeBool();
    double timestamp = decodeFloat();
    double value = decodeFloat();
    SlideFiltersEntry* recording = new SlideFiltersEntry(value, timestamp, connToFollow);
    return recording;
}

SlideFiltersEntry* DecoderSlideFilter::getAt(int position){
    if (current_position < position){
        current_position = position;
        lastDecodedEntry = decodeEntry();
    }
    return lastDecodedEntry;
}


void DecoderSlideFilter::addValue(DataItem data_item){
    while (mask->isNoData()) {
        column->addNoData();
        row_index++;
    }
    std::string value = Conversor::doubleToIntToString(data_item.value);
    column->addData(value);
    row_index++;
}


// Calculate approximation data from model parameters
void DecoderSlideFilter::decompress(std::vector<int> x_coords_vector){
#if CHECKS
    assert(mask->total_no_data + mask->total_data == data_rows_count);
#endif // END CHECKS
    row_index = 0;
    mask->reset();

    int unprocessed_rows = mask->total_data;
    int start_position = 0;
    while (unprocessed_rows > 0) {
        int current_window_size = (unprocessed_rows >= window_size) ? window_size : unprocessed_rows;

        int first_timestamp = x_coords_vector[0];
        for(int i = 0; i < x_coords_vector.size(); i++){
            x_coords_vector[i] -= first_timestamp - 1;
        }
        decompressWindow(x_coords_vector, start_position, current_window_size);
        start_position += current_window_size;

        unprocessed_rows -= current_window_size;
        if (unprocessed_rows > 0){
            x_coords_vector.erase(x_coords_vector.begin(), x_coords_vector.begin() + current_window_size);
        }
    }
}

void DecoderSlideFilter::decompressWindow(std::vector<int> x_coords_vector, int start_position, int current_window_size)
{
    SlideFiltersEntry slEntry1, slEntry2;
    DataItem inputEntry;

    current_position = start_position-1;

    if (x_coords_vector.size() == 1){
        slEntry1 = *getAt(start_position);
        inputEntry.timestamp = slEntry1.timestamp;
        inputEntry.value = slEntry1.value;
        addValue(inputEntry);
        return;
    }

    int position = start_position;
    double timeStamp = 0;
    int first_coord = x_coords_vector.at(0);

    Line* l = NULL;

    int x_coords_vector_index = 0;
    int i = 0;
    while (current_window_size > 0)
    {
        //Read compressed data
        if (i >= timeStamp)
        {
            slEntry1 = *getAt(position);

            if (slEntry1.connToFollow)//Connected
            {
                position++;
                slEntry2 = *getAt(position);

                //Go back for second reading
                if (slEntry2.connToFollow)
                {
                    timeStamp = slEntry2.timestamp - 1;
                }
                else
                {
                    position++;
                    timeStamp = slEntry2.timestamp;
                }
            }
            else //Disconnected
            {
                inputEntry.timestamp = slEntry1.timestamp;
                inputEntry.value = slEntry1.value;
                addValue(inputEntry);
                break;
            }

            //Create line go through two point in compressed data
            if (l != NULL)
            {
                delete l;
            }

            Point p1(slEntry1.value, slEntry1.timestamp);
            Point p2(slEntry2.value, slEntry2.timestamp);
            l = new Line(&p1, &p2);
        }

        i++;
        if (x_coords_vector[x_coords_vector_index] >= i + first_coord) { continue; }

        x_coords_vector_index++;
        //Get point on line at each corresponding time
        inputEntry.timestamp = i;
        inputEntry.value = l->getValue(inputEntry.timestamp);
        addValue(inputEntry);
        current_window_size--;
    }
    delete l;
}
