
#include "slide_filter_window.h"
#include "conversor.h"
#include "iostream"
#include "assert.h"

SlideFilterWindow::SlideFilterWindow(int total_data_rows_, int epsilon_){
    total_data_rows = total_data_rows_;
    data.reserve(total_data_rows);
    length = 0;
    epsilon = epsilon_;
}

SlideFilterWindow::SlideFilterWindow(CoderSlideFilter* coder_){
    coder = coder_;
}

void SlideFilterWindow::addDataItem(int timestamp, std::string value){
    int new_timestamp = (length == 0) ? 0 : data[length-1].timestamp + timestamp;
    int value_int = Conversor::stringToInt(value);
    data[length] = DataItem(value_int, new_timestamp);
    length++;
}

void SlideFilterWindow::addDataItemTwo(int timestamp, int value){
    int new_timestamp = (length == 0) ? 1 : timestamp + 1;
    data[length] = DataItem(value, new_timestamp);
    length++;
}

int SlideFilterWindow::getDataLength() {
    return length;
}

DataItem SlideFilterWindow::getAt(int pos) {
    return data[pos];
}

int SlideFilterWindow::getEsp() {
    return epsilon;
}
