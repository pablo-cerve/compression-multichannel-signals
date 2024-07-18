
#include "coder_pca.h"

#include "assert.h"
#include "conversor.h"

void CoderPCA::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
}

void CoderPCA::codeColumnBefore(){
    window = createWindow();
}

void CoderPCA::codeColumnWhile(std::string csv_value){
    if (Constants::isNoData(csv_value)) { return; } // skip no_data

    window->addValue(csv_value);
    if (window->isFull()) { codeWindow(window); }
}

void CoderPCA::codeColumnAfter(){
    if (!window->isEmpty()) {
        assert(!window->isFull());
        codeWindow(window);
    }
}

PCAWindow* CoderPCA::createWindow(){
    int epsilon = getEpsilon(epsilons_vector, column_index);
    return new PCAWindow(window_size, epsilon);
}

void CoderPCA::codeWindow(PCAWindow* window){
    if (window->has_constant_value) { codeConstantWindow(window);     }
    else                            { codeNonConstantWindow(window);  }
    window->clearWindow();
}

void CoderPCA::codeConstantWindow(PCAWindow* window){
    codeBit(0);
    codeValueRaw(window->constant_value);
}

void CoderPCA::codeNonConstantWindow(PCAWindow* window){
    codeBit(1);
    for(int i=0; i < window->length; i++){
        std::string csv_value = window->getElement(i);
        codeValueRaw(csv_value);
    }
}

std::vector<int> CoderPCA::codeTimeDelta(CoderPCA* coder, int window_size){
    int epsilon = 0; // timestamps column is always encoded lossless
    PCAWindow* window = new PCAWindow(window_size, epsilon);

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

        // same code as CoderPCA::codeColumnWhile
         window->addValue(csv_value);
        if (window->isFull()) { coder->codeWindow(window); }
    }
    // same code as CoderPCA::codeColumnAfter
    if (!window->isEmpty()) {
        assert(!window->isFull());
        coder->codeWindow(window);
    }
    return time_delta_vector;
}
