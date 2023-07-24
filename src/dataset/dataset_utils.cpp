
#include "dataset_utils.h"

#include "assert.h"
#include "string_utils.h"
#include "conversor.h"
#include <math.h>
#include "vector_utils.h"

const std::vector<std::string>
        DatasetUtils::DATASET_NAMES = {"IRKIS", "NOAA-SST", "NOAA-ADCP", "ElNino", "SolarAnywhere",
                                       "NOAA-SPC-hail", "NOAA-SPC-tornado", "NOAA-SPC-wind"};

const int DatasetUtils::MAX_DATA_ROWS_BITS = 24;

const std::vector<std::string>
        DatasetUtils::UNITS = {"seconds", "minutes", "hours", "dimensionless", "degrees Celsius", "m/s",
                               "coord. degrees", "percentage", "W/m2", "1/100 inch", "mph"};

const std::vector<int> DatasetUtils::SCALES = {1, 10, 100, 1000};

const std::string DatasetUtils::METADATA_HEADER = "METADATA:";
const std::string DatasetUtils::METADATA_COLUMNS = "COLUMNS,UNIT,SCALE,MINIMUM,MAXIMUM";
const std::string DatasetUtils::DATA_HEADER = "DATA:";

bool DatasetUtils::validDatasetName(std::string dataset_name){
    return VectorUtils::vectorIncludesString(DATASET_NAMES, dataset_name);
}

bool DatasetUtils::validDataRowsCount(int data_rows_count){
    int max_data_rows = pow(2, MAX_DATA_ROWS_BITS);
    return (0 < data_rows_count && data_rows_count < max_data_rows);
}

bool DatasetUtils::validUnit(std::string unit){
    return VectorUtils::vectorIncludesString(UNITS, unit);
}

bool DatasetUtils::validScale(std::string scale){
    int scale_int = Conversor::stringToInt(scale);
    return VectorUtils::vectorIncludesInt(SCALES, scale_int);
}

std::string DatasetUtils::getDatasetNameFromFilePath(std::string file_path){
    if (StringUtils::stringCointainsSubstring(file_path, "[1]IRKIS"))                 { return "IRKIS"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[2]NOAA-SST"))         { return "NOAA-SST"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[3]NOAA-ADCP"))        { return "NOAA-ADCP"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[4]SolarAnywhere"))    { return "SolarAnywhere"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[5]ElNino"))           { return "ElNino"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[6]NOAA-SPC-hail"))    { return "NOAA-SPC-hail"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[7]NOAA-SPC-tornado")) { return "NOAA-SPC-tornado"; }
    else if (StringUtils::stringCointainsSubstring(file_path, "[8]NOAA-SPC-wind"))    { return "NOAA-SPC-wind"; }
    else { return "invalid"; }
}
