
#ifndef CPP_PROJECT_SLIDE_FILTER_WINDOW_H
#define CPP_PROJECT_SLIDE_FILTER_WINDOW_H

#include <string>
#include <vector>
#include "Line.h"
#include "window.h"

class CoderSlideFilter;

class SlideFilterWindow: public Window {

private:
    std::vector<DataItem> data;
    int total_data_rows;
    CoderSlideFilter* coder;

public:
    SlideFilterWindow(int total_data_rows_, int epsilon_);
    SlideFilterWindow(CoderSlideFilter* coder_);
    void addDataItem(int timestamp, std::string value);
    void addDataItemTwo(int timestamp, int value_int);
    int getDataLength();
    DataItem getAt(int pos);
    int getEsp();
};

#endif //CPP_PROJECT_SLIDE_FILTER_WINDOW_H
