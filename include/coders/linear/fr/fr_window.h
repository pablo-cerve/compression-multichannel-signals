
#ifndef CPP_PROJECT_FR_WINDOW_H
#define CPP_PROJECT_FR_WINDOW_H

#include "DataItem.h"
#include <vector>
#include <string>
#include "window.h"

class FRWindow: public Window {

private:
    std::vector<DataItem> data;
    void getIndexes(std::vector<int> & array, int first_index, int last_index);
    bool violatedConstraint(int first_index, int last_index);

public:
    FRWindow(int window_size_, int epsilon_);
    void addDataItem(int timestamp, std::string value);
    bool isFull();
    bool isEmpty();
    void clear();
    std::vector<DataItem> getItems();
};

#endif //CPP_PROJECT_FR_WINDOW_H
