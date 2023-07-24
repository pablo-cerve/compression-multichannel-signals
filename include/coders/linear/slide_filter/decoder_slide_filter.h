
#ifndef CPP_PROJECT_DECODER_SLIDE_FILTER_H
#define CPP_PROJECT_DECODER_SLIDE_FILTER_H

#include "decoder_cols.h"
#include "SlideFiltersEntry.h"
#include "slide_filter_window.h"
#include "DynArray.h"
#include "DataStream.h"

class DecoderSlideFilter: public DecoderCols {

private:
    Column* column;
    int current_position;
    SlideFiltersEntry* lastDecodedEntry;

    std::vector<std::string> decodeDataColumn(bool mask_mode) override;
    SlideFiltersEntry* decodeEntry();
    SlideFiltersEntry* getAt(int position);
    void addValue(DataItem data_item);
    void decompress(std::vector<int> x_coords_vector);
    void decompressWindow(std::vector<int> x_coords_vector, int start_position, int current_window_size);

public:
    using DecoderCols::DecoderCols;
};

#endif //CPP_PROJECT_DECODER_SLIDE_FILTER_H
