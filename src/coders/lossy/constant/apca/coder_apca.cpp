
#include "coder_apca.h"

#include "assert.h"
#include "conversor.h"

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

std::vector<int> CoderAPCA::codeTimeDelta(CoderCommon* coder, int window_size){
    int epsilon = 0; // timestamps column is always encoded lossless
    bool mask_mode = false;
    APCAWindow* window = new APCAWindow(window_size, epsilon, mask_mode);

    CSVReader* input_csv = coder->input_csv;

    std::vector<int> time_delta_vector{};
    input_csv->goToFirstDataRow(0);
    bool first_value = true;
    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        int csv_value_int = Conversor::stringToInt(csv_value);
        time_delta_vector.push_back(csv_value_int);

        if (first_value){
            // the first value is always 0, no need to encode anything
            assert(csv_value == "0");
            first_value = false;
            continue;
        }
       
        // same code as CoderAPCA::codeColumnWhile
        if (!window->conditionHolds(csv_value)){
            // cannot call coder->codeWindow because coder is CoderCommon*
            coder->codeWindowLength((Window*) window);
            coder->codeValueRaw(window->constant_value);
            window->addFirstValue(csv_value);
        }
    }
    // same code as CoderAPCA::codeColumnAfter
    if (!window->isEmpty()){
        // cannot call coder->codeWindow because coder is CoderCommon*
        coder->codeWindowLength((Window*) window);
        coder->codeValueRaw(window->constant_value);
    }
    return time_delta_vector;
}