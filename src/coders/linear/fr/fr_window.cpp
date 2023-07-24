
#include "fr_window.h"
#include "conversor.h"
#include "assert.h"
#include <iostream>
#include "Line.h"
#include "math_utils.h"
#include "vector_utils.h"

FRWindow::FRWindow(int window_size_, int epsilon_): Window(window_size_, epsilon_){
    length = 0;
    data.reserve(window_size_);
}

void FRWindow::clear(){
    length = 0;
}

void FRWindow::addDataItem(int timestamp, std::string value){
    assert(length <= window_size);
    int new_timestamp = (length == 0) ? 0 : data[length-1].timestamp + timestamp;
    int value_int = Conversor::stringToInt(value);
    data[length] = DataItem(value_int, new_timestamp);
    length++;
}

bool FRWindow::isFull(){
    return length == window_size;
}

bool FRWindow::isEmpty(){
    return length == 0;
}

std::vector<DataItem> FRWindow::getItems(){
    std::vector<int> array{};
    getIndexes(array, 0, length - 1);
//    VectorUtils::printIntVector(array);

    std::vector<DataItem> result;
    for(int i=0; i < array.size(); i++) {
        int position = array[i];
        DataItem data_item(data[position].value, position);
        result.push_back(data_item);
    }
    return result;
}

void FRWindow::getIndexes(std::vector<int> & array, int first_index, int last_index){
    if (!VectorUtils::vectorIncludesInt(array, first_index)) { array.push_back(first_index); }
    if ((first_index + 1 < last_index) && violatedConstraint(first_index, last_index)) {
        // displace segment
        int half = MathUtils::half(first_index, last_index);
        getIndexes(array, first_index, half);
        getIndexes(array, half, last_index);
    }
    if (!VectorUtils::vectorIncludesInt(array, last_index)) { array.push_back(last_index); }
}

bool FRWindow::violatedConstraint(int first_index, int last_index){
    DataItem first_item, last_item;
    first_item = data[first_index];
    last_item = data[last_index];

    Point* first_point = new Point(first_item);
    Point* last_point = new Point(last_item);

    if (first_point->x == last_point->x){ // vertical line with equation x = first_point.x
        return (first_index + 1 != last_index); // the constraint is violated if there are points in between
    }

    Line* line = new Line(first_point, last_point);
    Point* point;
    for(int i=first_index+1; i < last_index; i++){
        point = new Point(data[i]);
        double dis = line->getYDistanceToDot(point);
        if (dis > epsilon){
            return true;
        }
    }
    return false;
}
