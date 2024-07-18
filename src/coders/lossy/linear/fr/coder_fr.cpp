
#include "conversor.h"
#include "coder_fr.h"

#include "math_utils.h"
#include "conversor.h"
#include "assert.h"

void CoderFR::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
    window_size_bit_length = MathUtils::windowSizeBitLength(window_size);
}

void CoderFR::codeColumnBefore(){
    delta_sum = 0;
    window = createWindow();
}

void CoderFR::codeColumnWhile(std::string csv_value) {
    delta_sum += time_delta_vector[row_index]; // >= 0
    if (Constants::isNoData(csv_value)) { return; } // skip no_data
    window->addDataItem(delta_sum, csv_value);
    if (window->isFull()){
        codeWindow();
        window->clear();
    }
    delta_sum = 0;
}

void CoderFR::codeColumnAfter() {
    if (window->isEmpty()) { return; }
    assert(!window->isFull());
    codeWindow();
}

FRWindow* CoderFR::createWindow(){
    int epsilon = getEpsilon(epsilons_vector, column_index);
    return new FRWindow(window_size, epsilon);
}

void CoderFR::codeWindow(){
    std::vector<DataItem> items = window->getItems();

    int size = items.size(); // 1 <= size <= window_size
    assert(size <= window_size);
    for(int i=0; i < size; i++){
        DataItem item = items[i];
        codeItem(item, i);
    }
}

void CoderFR::codeItem(DataItem item, int index){
    int value = Conversor::doubleToInt(item.value);
    std::string value_str = Conversor::intToString(value);
    codeValueRaw(value_str);
    // we always code the value in the first index, so we don't have to code its index
    if (index == 0) { return; }
    codeInt(item.timestamp - 1, window_size_bit_length); // 1 <= index <= window_size
}
