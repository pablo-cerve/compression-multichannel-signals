
#ifndef CPP_PROJECT_CODER_UTILS_H
#define CPP_PROJECT_CODER_UTILS_H

#include <vector>
#include <string>
#include "constants.h"
#include "mask.h"
#include <iostream>

class CoderUtils {

public:
    static std::string mapValue(std::string csv_value, int offset);
    static int mapValueInt(std::string csv_value, int offset);
    static std::string unmapValueInt(double value, int offset);
    static std::string unmapValue(std::string value, int offset);

    static std::vector<int> createXCoordsVector(std::vector<int> time_delta_vector, int window_size, int row_index);
    static std::vector<int> createXCoordsVector(Mask* mask, std::vector<int> time_delta_vector, int window_size);
    static std::vector<int> createXCoordsVectorMaskMode(Mask* mask, std::vector<int> time_delta_vector, int init_delta_sum);
    static std::vector<int> createXCoordsVectorMaskModeSF(Mask* mask, std::vector<int> time_delta_vector, int init_delta_sum);
    static int calculateDelta(int delta, int row_index);
};

#endif //CPP_PROJECT_CODER_UTILS_H
