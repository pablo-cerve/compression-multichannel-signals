
#ifndef CPP_PROJECT_HEADER_CODER_H
#define CPP_PROJECT_HEADER_CODER_H

#include <iostream>
#include "coder_common.h"
#include "csv_reader.h"
#include "dataset.h"
#include "dataset_utils.h"


class HeaderCoder {

public:
    HeaderCoder(CSVReader* input_csv_, BitStreamWriter* output_file_); // test_mode = true
    HeaderCoder(CSVReader* input_csv_, CoderCommon* coder_common_); // test_mode = false
    int codeHeader(Dataset* dataset);

private:
    CSVReader* input_csv;
    BitStreamWriter* output_file;
    CoderCommon* coder_common;
    bool test_mode;

    std::string codeDatasetName();
    int codeDataRowsCount();
    void codeFirstTimestamp();

    void codeMetadata(std::vector<std::string> & column_names, std::vector<Range*> & ranges);
    void codeMetadataRow(int i,
                         std::string line,
                         std::vector<std::string> & column_names,
                         std::vector<Range*> & ranges);

    int codeColumnNames(std::vector<std::string> column_names);
    void codeLine(std::string line);
    void checkSizeAndKey(std::vector<std::string> line_vector, int expected_size, std::string expected_key);
};

#endif //CPP_PROJECT_HEADER_CODER_H
