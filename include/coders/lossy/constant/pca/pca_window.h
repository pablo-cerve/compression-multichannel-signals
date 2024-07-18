
#ifndef CPP_PROJECT_PCA_WINDOW_H
#define CPP_PROJECT_PCA_WINDOW_H

#include <vector>
#include <string>
#include "constants.h"
#include "window.h"

class PCAWindow: public Window {

private:
    int min;
    int max;

    void updateMinAndMax(int x_int);
    void addNonFirstValue(std::string x); // PRE: !isFull() && !isEmpty()
    void addFirstValue(std::string x); // PRE: isEmpty()
    void updateConstantValue();

public:
    bool has_constant_value;
    std::vector<std::string> *array;

    PCAWindow(int window_size_, int epsilon_);
    void addValue(std::string x); // PRE: !isFull()
    bool isFull();
    bool isEmpty();
    void clearWindow();
    std::string getElement(int pos); // PRE: pos < length

};

#endif //CPP_PROJECT_PCA_WINDOW_H
