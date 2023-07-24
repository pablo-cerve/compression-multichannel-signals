
#include "coder_apca.h"

void CoderAPCA::setCoderParams(int window_size_, std::vector<int> epsilons_vector_, bool mask_mode_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
    mask_mode = mask_mode_;
}

void CoderAPCA::codeColumnBefore(){
    int epsilon = getEpsilon(epsilons_vector, column_index);
    window = new APCAWindow(window_size, epsilon, mask_mode);
}

void CoderAPCA::codeColumnWhile(std::string csv_value){
if (mask_mode) {
    if (Constants::isNoData(csv_value)) { return; } // skip no_data
}
    if (!window->conditionHolds(csv_value)){
        codeWindow(window);
        window->addFirstValue(csv_value);
    }
}

void CoderAPCA::codeColumnAfter(){
    if (!window->isEmpty()){ codeWindow(window); }
}

void CoderAPCA::codeWindow(APCAWindow* window){
    codeWindowLength((Window*) window);
    codeValueRaw(window->constant_value);
}
