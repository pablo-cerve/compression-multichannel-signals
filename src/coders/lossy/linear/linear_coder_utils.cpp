
#include "linear_coder_utils.h"

std::vector<int> LinearCoderUtils::createXCoordsVectorCA(DecoderCommon* decoder, int window_size, int row_index, int nodata_sum){
#if CHECKS
    assert(window_size > 1);
#endif
    std::vector<int> result;
    result.push_back(0);

    int current_sum = nodata_sum;
    int time_delta;
    int window_index = 1;
    int time_delta_index = row_index;
    while(window_index < window_size){
        time_delta_index++;
        time_delta = decoder->time_delta_vector.at(time_delta_index);
        current_sum += time_delta;

        if (result.size() > 1 && decoder->mask->isNoData()){
            result.push_back(-1);
        }
        else {
            result.push_back(current_sum);
            window_index++;
        }
    }
    return result;
}

std::vector<int> LinearCoderUtils::createXCoordsVectorPWLH(DecoderCommon* decoder, int window_size, int row_index){
#if CHECKS
    assert(window_size > 1);
#endif
    std::vector<int> result;
    result.push_back(0);

    int current_sum = 0;
    int time_delta;
    int window_index = 1;
    int time_delta_index = row_index;
    while(window_index < window_size){
        time_delta_index++;
        time_delta = decoder->time_delta_vector.at(time_delta_index);
        current_sum += time_delta;

        if (decoder->mask->isNoData()){
            result.push_back(-1);
        }
        else {
            result.push_back(current_sum);
            window_index++;
        }
    }
    return result;
}

void LinearCoderUtils::addPointsWithNoData(Column* column,
                                           int window_size,
                                           std::vector<std::string> decoded_points,
                                           std::vector<int> x_coords_with_nodata){
#if CHECKS
    assert(window_size == decoded_points.size());
    assert(window_size <= x_coords_with_nodata.size());
    assert(x_coords_with_nodata.at(0) != -1);
#endif
    int i = -1;
    int window_index = 0;
    while (window_index < window_size){
        i++;
        if (x_coords_with_nodata[i] == -1){
            column->addNoData();
        }
        else {
            std::string decoded_point = decoded_points[window_index];
            column->addData(decoded_point);
            window_index++;
        }
    }
}
