
#ifndef CPP_PROJECT_WINDOW_CA_H
#define CPP_PROJECT_WINDOW_CA_H

#include <string>
#include "Line.h"
#include "window.h"

class CAWindow: public Window {

private:
    Point* archived_value;
    Point* snapshot_value;
    Line* s_min;
    Line* s_max;

    Line* sMin(Point* point1, Point* point2, int epsilon);
    Line* sMax(Point* point1, Point* point2, int epsilon);

public:
    int x_coord;
    bool nan_window;

    CAWindow(int window_size_, int epsilon_);
    bool isFull();
    bool isEmpty();
    void createNonNanWindow(std::string incoming_value_str, int incoming_value);
    void createNanWindow();
    bool conditionHolds(int x_delta, int x_int, std::string x);
    void setWindow(int x_delta, int x_int, std::string x);
    void increaseLength();
};

#endif //CPP_PROJECT_WINDOW_CA_H
