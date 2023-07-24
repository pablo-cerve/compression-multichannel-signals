
#ifndef CPP_PROJECT_TIME_DELTA_CODER_H
#define CPP_PROJECT_TIME_DELTA_CODER_H

#include "coder_common.h"

class TimeDeltaCoder {

public:
    static std::vector<int> code(CoderCommon* coder);
    static int getWindowSize(std::string dataset_name);
};

#endif //CPP_PROJECT_TIME_DELTA_CODER_H
