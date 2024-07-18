
#include "coder_pwlh.h"

#include <cfloat>


void CoderPWLH::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    integer_mode = coder_name == "PWLH*Int";
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
}

void CoderPWLH::codeColumnBefore(){
    delta_sum = 0;
    window = createWindow();
}

void CoderPWLH::codeColumnWhile(std::string csv_value){
    delta_sum += time_delta_vector[row_index]; // >= 0
    if (Constants::isNoData(csv_value)) { return; } // skip no_data

    if (!window->conditionHolds(csv_value, delta_sum)){
        codeWindow(window);
        window->addFirstValue(csv_value);
    }
    delta_sum = 0;
}

void CoderPWLH::codeColumnAfter(){
    if (!window->isEmpty()){ codeWindow(window); }
}

PWLHWindow* CoderPWLH::createWindow(){
    float epsilon = getEpsilon(epsilons_vector, column_index);
    if (integer_mode && epsilon > 0){ epsilon -= 0.5; }
    Range* column_range = dataset->column_code->range;
    return new PWLHWindow(window_size, epsilon, column_range, integer_mode);
}

void CoderPWLH::codeWindow(PWLHWindow* window){
    codeWindowLength((Window*) window);
    (integer_mode) ? codeWindowInt(window) : codeWindowDouble(window);
}

void CoderPWLH::codeWindowDouble(PWLHWindow* window){
    if (window->length > 1) {
        float point1_y = window->getPoint1Y();
        float point2_y = window->getPoint2Y();
        codeFloat(point1_y);
        codeFloat(point2_y);
        return;
    }
    // window.length == 1 => this code can only run the last time codeWindow is called
    // IMPORTANT: window.constant_value_float is an int casted as a float
    codeFloat(window->constant_value_float); // no need to code another value
}

void CoderPWLH::codeWindowInt(PWLHWindow* window){
    if (window->length > 1){
        std::string point1_y = window->getPoint1YIntegerMode();
        std::string point2_y = window->getPoint2YIntegerMode();
        codeValueRaw(point1_y);
        codeValueRaw(point2_y);
        return;
    }
    // window.length == 1 => this code can only run the last time codeWindow is called
    codeValueRaw(window->constant_value); // no need to code another value
}
