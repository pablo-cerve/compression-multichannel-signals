
#include "pca_window.h"

#include <iostream>
#include <math_utils.h>
#include "conversor.h"
#include "constant_coder_utils.h"

PCAWindow::PCAWindow(int window_size_, int epsilon_): Window(window_size_, epsilon_){
    array = new std::vector<std::string>;
    length = 0;
    has_constant_value = true;
    min = 0;
    max = 0;
}

void PCAWindow::updateMinAndMax(int x_int){
    if (x_int < min)      { min = x_int; }
    else if (x_int > max) { max = x_int; }
}

void PCAWindow::updateConstantValue(){
    constant_value = ConstantCoderUtils::calculateConstantValue(min, max);
}

void PCAWindow::addValue(std::string x){
    if (isEmpty()) { addFirstValue(x);    }
    else           { addNonFirstValue(x); }
    array->push_back(x);
    length++;
}

void PCAWindow::addFirstValue(std::string x){
    int x_int = Conversor::stringToInt(x);
    min = x_int;
    max = x_int;
    constant_value = x;
}

void PCAWindow::addNonFirstValue(std::string x){
    if (!has_constant_value) { return; }

    int x_int = Conversor::stringToInt(x);
    updateMinAndMax(x_int);
    if (ConstantCoderUtils::validThreshold(min, max, epsilon)){
        updateConstantValue();
    }
    else {
        has_constant_value = false;
    }
}

bool PCAWindow::isFull(){
    return length == window_size;
}

bool PCAWindow::isEmpty(){
    return length == 0;
}

void PCAWindow::clearWindow(){
    array->clear();
    length = 0;
    has_constant_value = true;
}

std::string PCAWindow::getElement(int pos){
    return array->at(pos);
}
