
#ifndef CPP_PROJECT_MASK_H
#define CPP_PROJECT_MASK_H

#include <vector>
#include <iostream>
#include <assert.h>
#include "burst.h"

class Mask {

private:
    // creation variables
    std::vector<Burst*> bursts;
    Burst* current_burst;

    // iteration variables
    int current_index;
    bool burst_is_no_data;
    int burst_length;

    void setBurst(int index);

public:
    int total_no_data; // number of "nodata" entries
    int total_data; // number of non-"nodata" entries

    Mask();

    // Used in the mask decoders
    void add(Burst* burst);
    void add(bool no_data, int length);

    // Used by CoderGAMPS and DecoderOutput classes
    void add(bool no_data);
    void close();

    void print();

    void reset();
    bool isNoData();
};

#endif //CPP_PROJECT_MASK_H
