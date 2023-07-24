
#include "apca_window.h"

#include "constant_coder_utils.h"
#include "conversor.h"
#include "math_utils.h"
#include "iostream"

APCAWindow::APCAWindow(int window_size_, double epsilon_, bool mask_mode_): Window(window_size_, epsilon_){
    mask_mode = mask_mode_;
    length = 0;
    min = 0;
    max = 0;
    nan_window = false; // this argument is only used if mask_mode is false
}

bool APCAWindow::conditionHolds(std::string x){
    if (isEmpty()){ // this condition is only true the first time this method is called
        addFirstValue(x);
        return true;
    }
    else if (isFull()){
        return false;
    }
if (!mask_mode){
    if (Constants::isNoData(x)){
        if (nan_window){ length++; return true;  }
        else {                     return false; }
    }
    // x is a double
    if (nan_window) { return false; }
}
    double x_double = Conversor::stringToDouble(x);
    if (x_double < min) { return updateConstantValue(x_double, max); }
    if (x_double > max) { return updateConstantValue(min, x_double); }
    // min <= x_double <= max
    length++;
    return true;
}

bool APCAWindow::updateConstantValue(double new_min, double new_max){
    if (!ConstantCoderUtils::validThreshold(new_min, new_max, epsilon)) { return false; }

    // condition holds, update min, max and constant
    min = new_min; max = new_max;
    constant_value = ConstantCoderUtils::calculateConstantValue(min, max);
    length++;
    return true;
}

bool APCAWindow::isFull(){
    return length == window_size;
}

bool APCAWindow::isEmpty(){
    return length == 0;
}

void APCAWindow::addFirstValue(std::string x){
if (!mask_mode){
    if (Constants::isNoData(x)){
        nan_window = true;
        constant_value = Constants::NO_DATA;
        length = 1;
        return;
    }
    // x is an int
    nan_window = false;
}
    double x_double = Conversor::stringToDouble(x);
    min = x_double;
    max = x_double;
    constant_value = x;
    length = 1;
}