
#include "coder_ts2diff.h"

#include <assert.h>
#include "conversor.h"

void CoderTS2Diff::setWindowSize(int window_size_){
    window_size = window_size_;
}

void CoderTS2Diff::codeColumnBefore() {
    read_first_value = false;
    window = new TS2DiffWindow(window_size);
    if (!mask_mode) {
        no_data_value = dataset->column_code-> range->end + 1;
    }
}

void CoderTS2Diff::codeColumnWhile(std::string csv_value){
    int value;
    if (Constants::isNoData(csv_value)) {
        if (mask_mode) { return; } // skip gap

        value = no_data_value;
    }
    else {
        value = Conversor::stringToInt(csv_value);
    }

    if (!read_first_value) {
        read_first_value = true;
        codeValueRaw(csv_value);
    }
    else {
        int delta = value - previous_value;
        if (window->isFull()){ codeWindow(this, window); }
        window->addValue(delta);
    }
    previous_value = value;
}

void CoderTS2Diff::codeColumnAfter() {
    if (!window->isEmpty()){ codeWindow(this, window); }
}

void CoderTS2Diff::codeWindow(CoderTS2Diff* coder, TS2DiffWindow* window){
    window->substractMin();
    codeWindowMin(coder, window);
    int significant_bits = codeWindowSignificantBits(coder, window);

    if (significant_bits > 0){
        for(int i=0; i < window->values->size(); i++){
            int delta = window->values->at(i);
            coder->codeInt(delta, significant_bits);
        }
    }
    window->clearWindow();
}

void CoderTS2Diff::codeWindowMin(CoderTS2Diff* coder, TS2DiffWindow* window){
    int min = window->getMin();
    int value = min;
    if (min > 0){
        coder->codeBit(0);
    }
    else {
        coder->codeBit(1);
        value = -min;
    }
    coder->codeInt(value, coder->dataset->bits());
}

int CoderTS2Diff::codeWindowSignificantBits(CoderTS2Diff* coder, TS2DiffWindow* window){
    int significant_bits = window->calculateSignificantBits();
    // TODO: inefficient, calculate this once for each column
    int bits_needed_to_code_significant_bits = MathUtils::bitLength(coder->dataset->bits());
    coder->codeInt(significant_bits, bits_needed_to_code_significant_bits);
    return significant_bits;
}

std::vector<int> CoderTS2Diff::codeTimeDelta(CoderTS2Diff* coder, int window_size){
    CSVReader* input_csv = coder->input_csv;

    std::vector<int> time_delta_vector{};
    input_csv->goToFirstDataRow(0);
    bool first_value = true;
    TS2DiffWindow* window = new TS2DiffWindow(window_size);

    while (input_csv->continue_reading){
        std::string csv_delta = input_csv->readNextValue();
        int delta = Conversor::stringToInt(csv_delta);
        time_delta_vector.push_back(delta);

        if (first_value){
            // the first value is always 0, no need to encode anything
            assert(csv_delta == "0");
            first_value = false;
            continue;
        }

        // code similar to CoderTS2Diff::codeColumnWhile
        if (window->isFull()){ codeWindow(coder, window); }
        window->addValue(delta);
    }
    // same code as CoderAPCA::codeColumnAfter
    if (!window->isEmpty()){ codeWindow(coder, window); }
    return time_delta_vector;
}