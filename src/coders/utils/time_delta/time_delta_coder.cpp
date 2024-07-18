
#include "time_delta_coder.h"

#include "vector_utils.h"
#include "coder_utils.h"
#include "coder_base.h"
#include "coder_gorilla.h"
#include "coder_ts2diff.h"
#include "coder_pca.h"
#include "coder_apca.h"

std::vector<int> TimeDeltaCoder::code(CoderCommon* coder){
    std::vector<int> column;
    
    if (coder->isCoder("Base")) {
        column = CoderBase::codeTimeDelta((CoderBase*) coder);
    }
    else if (coder->isCoder("Gorilla")){
        column = CoderGorilla::codeTimeDelta((CoderGorilla*) coder);
    }
    else if (coder->isCoder("TS2Diff")){
        // use the window_size passed as argument
        int window_size = coder->window_size;
        column = CoderTS2Diff::codeTimeDelta((CoderTS2Diff*) coder, window_size);
    }
    else if (coder->isCoder("PCA*")){
        // use the window_size passed as argument
        int window_size = coder->window_size;
        column = CoderPCA::codeTimeDelta((CoderPCA*) coder, window_size);
    }
    else {
        // use a fixed window_size, based on experimental results
        int window_size = getWindowSize(coder->dataset->dataset_name);
        column = CoderAPCA::codeTimeDelta(coder, window_size);
    }
    return column;
}

int TimeDeltaCoder::getWindowSize(std::string dataset_name){
    std::vector<std::string> window256 = {"IRKIS", "NOAA-SST", "NOAA-ADCP", "ElNino", "SolarAnywhere"};
    std::vector<std::string> window4 = {"NOAA-SPC-hail", "NOAA-SPC-tornado", "NOAA-SPC-wind"};

    int window_size = 32;
    if (VectorUtils::vectorIncludesString(window256, dataset_name)){
        window_size = 256;
    }
    else if (VectorUtils::vectorIncludesString(window4, dataset_name)){
        window_size = 4;
    }
    return window_size;
}
