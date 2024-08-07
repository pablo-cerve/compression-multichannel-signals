#include "assert.h"

#include "coder_ca.h"
#include "conversor.h"

void CoderCA::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
}

void CoderCA::codeColumnBefore(){
    int epsilon = getEpsilon(epsilons_vector, column_index);
    window = new CAWindow(window_size, epsilon);
}

void CoderCA::codeColumnWhile(std::string csv_value){
    delta_sum += time_delta_vector[row_index]; // >= 0
    if (Constants::isNoData(csv_value)) { return; } // skip no_data

    processValue(csv_value);
    delta_sum = 0;
}

void CoderCA::codeColumnAfter(){
    if (window->length > 0){
        codeWindow();
    }
}


void CoderCA::processValue(std::string x){
    int x_int = Conversor::stringToInt(x);

    if (window->isEmpty()){
        if (window->nan_window){ // this condition can only be true on the first iteration
            codeArchivedValueAndCreateNonNanWindow(x, x_int);
            return;
        }

        if (delta_sum == 0) {
            codeArchivedValueAndCreateNonNanWindow(x, x_int);
        }
        else {
            window->setWindow(delta_sum, x_int, x);
        }
        return;
    }

    if (window->isFull()){
        codeWindow();
        codeArchivedValueAndCreateNonNanWindow(x, x_int);
        return;
    }

    if (delta_sum == 0 || not window->conditionHolds(delta_sum, x_int, x)){
        codeWindow(); // code non-nan window
        codeArchivedValueAndCreateNonNanWindow(x, x_int);
    }
}

void CoderCA::codeArchivedValueAndCreateNonNanWindow(std::string archived_value, int archived_value_int){
    codeValueRaw(archived_value);
    window->createNonNanWindow(archived_value, archived_value_int);
}

void CoderCA::codeWindow(){
    codeWindow(window->length, window->constant_value);
}

void CoderCA::codeWindow(int window_length, std::string window_value){
    assert(window_length > 0);
    codeValueRaw(window_value);
    codeInt(window_length - 1, window->window_size_bit_length);
}
