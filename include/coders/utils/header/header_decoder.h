
#ifndef CPP_PROJECT_HEADER_DECODER_H
#define CPP_PROJECT_HEADER_DECODER_H

#include <iostream>
#include "bit_stream_reader.h"
#include "csv_writer.h"
#include "dataset.h"
#include "dataset_utils.h"


class HeaderDecoder {

public:
    HeaderDecoder(BitStreamReader* input_file_, CSVWriter* output_csv_);
    Dataset* decodeHeader(int & data_rows_count);

private:
    BitStreamReader* input_file;
    CSVWriter* output_csv;

    std::string decodeDatasetName();
    int decodeDataRowsCount();
    void decodeFirstTimestamp();

    std::vector<Range*> decodeMetadata();
    void decodeMetadataRow(std::string line, std::vector<Range*> & ranges);

    int decodeColumnNames();
    std::string decodeLine();
};

#endif //CPP_PROJECT_HEADER_DECODER_H
