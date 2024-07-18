
#include "ts2diff_window.h"

#include <assert.h>
#include "math_utils.h"
#include "vector_utils.h"


TS2DiffWindow::TS2DiffWindow(int window_size_){
    assert(window_size_ > 1);
    window_size = window_size_;
    values = new std::vector<int>;
    clearWindow();
}

void TS2DiffWindow::clearWindow(){
    values->clear();
    min = 0;
    max = 0;
}

bool TS2DiffWindow::isFull(){
    return values->size() == window_size;
}

bool TS2DiffWindow::isEmpty(){
    return values->size() == 0;
}

void TS2DiffWindow::addValue(int value){
    values->push_back(value);

    if (isEmpty()){
        min = value;
        max = value;
    }
    else if (value < min){
        min = value;
    }
    else if (value > max){
        max = value;
    }
}

void TS2DiffWindow::substractMin(){
    for(int i=0; i < values->size(); i++){
        values->at(i) -= min;
    }
    max -= min;
}

int TS2DiffWindow::getMin(){
    return min;
}

int TS2DiffWindow::calculateSignificantBits(){
    return MathUtils::bitLength(max);
}

void TS2DiffWindow::print(){
    std::cout << "WINDOW" << std::endl;
    std::cout << "min = " << min << std::endl;
    std::cout << "max = " << max << std::endl;
    std::cout << "values" << std::endl;
    VectorUtils::printIntVector(*values);
}