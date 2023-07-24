
#ifndef CPP_PROJECT_DATASET_UTILS_H
#define CPP_PROJECT_DATASET_UTILS_H

#include <vector>
#include "text_reader.h"
#include "range.h"
#include "os_utils.h"

class DatasetUtils {

public:
    static const std::vector<std::string> DATASET_NAMES;
    static const int MAX_DATA_ROWS_BITS;
    static const std::vector<std::string> UNITS;
    static const std::vector<int> SCALES;

    static const std::string METADATA_HEADER;
    static const std::string METADATA_COLUMNS;
    static const std::string DATA_HEADER;

    static bool validDatasetName(std::string dataset_name);
    static bool validDataRowsCount(int data_rows_count);
    static bool validUnit(std::string unit);
    static bool validScale(std::string scale);

    static std::string getDatasetNameFromFilePath(std::string file_path);
};

#endif //CPP_PROJECT_DATASET_UTILS_H
