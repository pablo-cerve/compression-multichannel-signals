
#ifndef CPP_PROJECT_RANGE_H
#define CPP_PROJECT_RANGE_H

#include <iostream>

struct Range {
    int begin;
    int end;

    Range(int b, int e) {
        begin = b;
        end = e;
    }

    bool insideRange(int value){
        return (begin <= value && value <= end);
    }

    bool operator == (const Range &range) const{
        return (begin == range.begin && end == range.end);
    }

    void print(){
        std::cout << "Range = [" << begin << "," << end << "]" << std::endl;
    }
};

#endif //CPP_PROJECT_RANGE_H
