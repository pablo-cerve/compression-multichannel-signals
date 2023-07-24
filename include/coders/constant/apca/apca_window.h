
#ifndef CPP_PROJECT_APCA_WINDOW_H
#define CPP_PROJECT_APCA_WINDOW_H

#include <string>
#include "constants.h"
#include "pca_window.h"

class APCAWindow: public Window {

private:
    double min;
    double max;
    bool nan_window;
    bool mask_mode;

public:
    APCAWindow(int window_size_, double epsilon_, bool mask_mode_);
    bool conditionHolds(std::string x);
    bool isFull();
    bool isEmpty();
    void addFirstValue(std::string x);
    bool updateConstantValue(double new_min, double new_max);
};

#endif //CPP_PROJECT_APCA_WINDOW_H
