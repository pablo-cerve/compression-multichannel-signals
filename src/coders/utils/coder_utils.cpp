
#include "coder_utils.h"
#include <iostream>
#include "conversor.h"

std::string CoderUtils::mapValue(std::string csv_value, int offset){
    if (Constants::isNoData(csv_value)){
        return csv_value;
    }
    int int_value = mapValueInt(csv_value, offset);
    return Conversor::intToString(int_value);
}

int CoderUtils::mapValueInt(std::string csv_value, int offset){
    int value = Conversor::stringToInt(csv_value) + offset;
#if CHECKS
    assert(value > 0);
#endif
    return value;
}

std::string CoderUtils::unmapValueInt(double value, int offset){
    if (value == Constants::NO_DATA_DOUBLE){
        return Constants::NO_DATA;
    }
    value -= offset;
    int val_int = Conversor::doubleToInt(value);
    return Conversor::intToString(val_int);
}

std::string CoderUtils::unmapValue(std::string value, int offset){
    if (Constants::isNoData(value)){
        return value;
    }
    int int_value = Conversor::stringToInt(value);
//#if CHECKS
//    assert(int_value >= 0);
//#endif
    int_value -= offset;
    return Conversor::intToString(int_value);
}

std::vector<int> CoderUtils::createXCoordsVector(std::vector<int> time_delta_vector, int window_size, int row_index){
    std::vector<int> result;
    int current_sum = 0;
    int time_delta;
    for(int i = 0; i < window_size; i++){
        time_delta = (i == 0) ? 0 : time_delta_vector.at(row_index + i);
        current_sum += time_delta;
        result.push_back(current_sum);
    }
    return result;
}

std::vector<int> CoderUtils::createXCoordsVector(Mask* mask, std::vector<int> time_delta_vector, int window_size){
    mask->reset();
    std::vector<int> result;
    int current_sum = 0;
    int time_delta;
    int i = 0;
    int row_index = -1;
    while(i < window_size) {
        row_index++;
        if (mask->isNoData()) { continue; } // ignore these values
        time_delta = (i == 0) ? 0 : time_delta_vector.at(row_index);
        current_sum += time_delta;
        result.push_back(current_sum);
        i++;
    }
    return result;
}

//
// Returns a vector with the time deltas between no "nodata" values.
// If there are "nodata" values in the middle, their time delta is still considered.
//
std::vector<int> CoderUtils::createXCoordsVectorMaskMode(Mask* mask, std::vector<int> time_delta_vector, int init_delta_sum){
#if CHECKS
    assert(time_delta_vector.size() == mask->total_data + mask->total_no_data);
#endif
    mask->reset();
    std::vector<int> result;
    int delta_sum = init_delta_sum;
    for(int i=0; i < mask->total_data + mask->total_no_data; i++){
        delta_sum += time_delta_vector.at(i);
        if (mask->isNoData()) { continue; } // skip no_data
        result.push_back(delta_sum);
    }
    return result;
}

//
// Same as the previous method but converting 0 jumps to 1 jumps
//
std::vector<int> CoderUtils::createXCoordsVectorMaskModeSF(Mask* mask, std::vector<int> time_delta_vector, int init_delta_sum){
#if CHECKS
    assert(time_delta_vector.size() == mask->total_data + mask->total_no_data);
#endif
    mask->reset();
    std::vector<int> result;
    int delta_sum = init_delta_sum;
    for(int i=0; i < mask->total_data + mask->total_no_data; i++){
        int delta = time_delta_vector.at(i);
        if (mask->isNoData()) {
            delta_sum += delta; // delta >= 0
            continue; // skip no_data
        }
        delta_sum += calculateDelta(delta, i);
        result.push_back(delta_sum);
    }
    return result;
}

int CoderUtils::calculateDelta(int delta, int row_index){
    // if row_index == 0 then delta = 0
    // if row_index > 0 then delta >= 1
    return ((delta == 0 && row_index != 0) ? 1 : delta);
}
