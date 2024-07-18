
#ifndef CPP_PROJECT_LINEAR_CODER_UTILS_H
#define CPP_PROJECT_LINEAR_CODER_UTILS_H

#include "constants.h"
#include "decoder_cols.h"

class LinearCoderUtils {

public:
    static std::vector<int> createXCoordsVectorCA(DecoderCommon* decoder, int window_size, int row_index, int nodata_sum);
    static std::vector<int> createXCoordsVectorPWLH(DecoderCommon* decoder, int window_size, int row_index);
    static void addPointsWithNoData(Column* column, int window_size, std::vector<std::string> decoded_points,
                                    std::vector<int> x_coords_with_nodata);
};

#endif //CPP_PROJECT_LINEAR_CODER_UTILS_H
