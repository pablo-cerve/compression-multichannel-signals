
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
//    std::cout << "(length, value_int, new_timestamp) = (" << length << ", " << value_int << ", " << new_timestamp << ")" << std::endl;
    length++;
}

void SlideFilterWindow::addDataItemTwo(int timestamp, int value){
    int new_timestamp = (length == 0) ? 1 : timestamp + 1;
    data[length] = DataItem(value, new_timestamp);
//    std::cout << "(length, value_int, new_timestamp) = (" << length << ", " << value << ", " << new_timestamp << ")" << std::endl;
    length++;
}

int SlideFilterWindow::getDataLength() {
    return length;
}

DataItem SlideFilterWindow::getAt(int pos) {
//    std::cout << "  p=" << pos << "  => (" << data[pos].timestamp << ", " << data[pos].value << ")" <<  std::endl;
    return data[pos];
}

int SlideFilterWindow::getEsp() {
    return epsilon;
}

//int SlideFilterWindow::getPosition(int timestamp) {
//    std::cout << "getPosition(" << timestamp << ")" << std::endl;
//    int index = 0;
//    int index_timestamp;
//    while (index < total_data_rows) {
//        index_timestamp = data[index].timestamp;
//        if (index_timestamp == timestamp) { return index; }
//        if (index_timestamp > timestamp) { assert(1 == 0); } // error
//        index++;
//    }
//}