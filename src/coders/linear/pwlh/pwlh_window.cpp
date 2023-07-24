
#include "pwlh_window.h"
#include "conversor.h"
#include "constants.h"


PWLHWindow::PWLHWindow(int window_size_,  float epsilon_, Range* range_, bool integer_mode_): Window(window_size_, epsilon_){
    range = range_;
    bucket = new LinearBucket(epsilon);
    length = 0; x_coord = 0;
    integer_mode = integer_mode_;
}

//
// PRE: x_delta >= 0
//
bool PWLHWindow::conditionHolds(std::string x, int x_delta){
    if (isEmpty()){ // this condition is only true the first time this method is called
        addFirstValue(x);
        return true;
    }
    else if (isFull()){
        return false;
    }
    // x is an integer
    if (x_delta == 0) { return false; } // when x_delta == 0 we need to create a new window

    int x_int = Conversor::stringToInt(x);
    int new_x_coord = x_coord + x_delta;
    bucket->addPointMOD(new_x_coord, x_int);

    if (bucket->checkEpsConstraint() && checkIntegerModeConstraint(new_x_coord)){ // bucket is valid
        length++; x_coord = new_x_coord;
        return true;
    }
    // bucket is invalid
    bucket->removePoint();
    bucket->getAproximatedLineMOD(p1, p2, x_coord);
#if CHECKS
    assert(p1.x == 0);
    assert(p2.x == x_coord);
#endif
    return false;
}

bool PWLHWindow::checkIntegerModeConstraint(int new_x_coord){
    bucket->getAproximatedLineMOD(p1, p2, new_x_coord);

    if (!integer_mode){ return true; }

    // this constraint is only checked when running in integer mode
    return range->insideRange(Conversor::doubleToInt(p1.y)) && range->insideRange(Conversor::doubleToInt(p2.y));
}

bool PWLHWindow::isFull(){
    return length == window_size;
}

bool PWLHWindow::isEmpty(){
    return length == 0;
}

void PWLHWindow::addFirstValue(std::string x){
    length = 1;
    // x is an integer
    x_coord = 0;
    int x_int = Conversor::stringToInt(x);
    if (bucket->getSize() != 0) { bucket->resetBucket(); }
    bucket->addPointMOD(0, (double) x_int); // should be the same as calling bucket->addPoint((double) x_int);
    constant_value = x;
    constant_value_float = (float) x_int;
}

float PWLHWindow::getPoint1Y(){
    return p1.y;
}

float PWLHWindow::getPoint2Y(){
    return p2.y;
}

std::string PWLHWindow::getPoint1YIntegerMode(){
    return Conversor::doubleToIntToString(p1.y);
}

std::string PWLHWindow::getPoint2YIntegerMode(){
    return Conversor::doubleToIntToString(p2.y);
}
