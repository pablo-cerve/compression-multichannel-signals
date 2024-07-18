
#ifndef CPP_PROJECT_TS2DIFF_WINDOW_H
#define CPP_PROJECT_TS2DIFF_WINDOW_H

#include <string>
#include <vector>

class TS2DiffWindow {

private:
    int window_size;
    int min;
    int max;

public:
    std::vector<int> *values;

    TS2DiffWindow(int window_size_);
    void clearWindow();
    bool isFull();
    bool isEmpty();
    void addValue(int value);
    void substractMin();
    int getMin();
    int calculateSignificantBits();

    void print();
    
};

#endif //CPP_PROJECT_TS2DIFF_WINDOW_H
