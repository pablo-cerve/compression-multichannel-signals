
#include "vector_utils.h"
#include "string_utils.h"
#include "time_delta_coder.h"
#include "conversor.h"
#include "coder_utils.h"
#include "apca_window.h"

std::vector<int> TimeDeltaCoder::code(CoderCommon* coder){
    int window_size = getWindowSize(coder->dataset->dataset_name);
    int epsilon = 0; // timestamps column is always encoded lossless
    bool mask_mode = false;
    APCAWindow* window = new APCAWindow(window_size, epsilon, mask_mode);

    CSVReader* input_csv = coder->input_csv;

    std::vector<int> time_delta_vector{};
    input_csv->goToFirstDataRow(0);
    bool first_value = true;
    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        if (first_value){
            assert(csv_value == "0");
            first_value = false;
        }
        // add int value to the time_delta_vector
        int csv_value_int = Conversor::stringToInt(csv_value);
        time_delta_vector.push_back(csv_value_int);

        if (!window->conditionHolds(csv_value)){
            coder->codeWindowLength((Window*) window);
            coder->codeValueRaw(window->constant_value);
            window->addFirstValue(csv_value);
        }
    }
    if (!window->isEmpty()){
        coder->codeWindowLength((Window*) window);
        coder->codeValueRaw(window->constant_value);
    }
    return time_delta_vector;
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
