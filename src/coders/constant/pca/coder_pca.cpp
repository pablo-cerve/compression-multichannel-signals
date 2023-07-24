
#include "coder_pca.h"
#include "assert.h"

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
