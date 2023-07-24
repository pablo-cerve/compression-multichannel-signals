
#include "header_decoder.h"

#include "assert.h"
#include "string_utils.h"
#include "dataset_utils.h"
#include "header_ts_utils.h"
#include "conversor.h"
#include <vector>

HeaderDecoder::HeaderDecoder(BitStreamReader* input_file_, CSVWriter* output_csv_){
    input_file = input_file_;
    output_csv = output_csv_;
}

/*
    DATASET:,IRKIS
    DATA ROWS:,26305
    FIRST TIMESTAMP:,2010-10-01 00:00:00
    METADATA:
    COLUMNS,UNIT,SCALE,MINIMUM,MAXIMUM
    Time Delta,minutes,1,0,13071
    -10cm_A,dimensionless,1000,0,600
    DATA:
    Time Delta,-10cm_A,-30cm_A,-50cm_A,-80cm_A,-120cm_A,-10cm_B,-30cm_B,-50cm_B,-80cm_B,-120cm_B
    0,N,N,N,N,N,N,N,N,N,N
    60,N,N,N,N,N,N,N,N,N,N
     . . .
*/
Dataset* HeaderDecoder::decodeHeader(int & data_rows_count){
    std::string dataset_name = decodeDatasetName();
    data_rows_count = decodeDataRowsCount();
    decodeFirstTimestamp();
    std::vector<std::string> row;

    row = {DatasetUtils::METADATA_HEADER};
    output_csv->writeRowDecoder(row);

    row = {DatasetUtils::METADATA_COLUMNS};
    output_csv->writeRowDecoder(row);

    std::vector<Range*> ranges = decodeMetadata();

    int data_columns_count = decodeColumnNames();

    Dataset* dataset = new Dataset();
    dataset->setHeaderValues(ranges, data_columns_count);
    dataset->setDatasetName(dataset_name);
    return dataset;
}

std::string HeaderDecoder::decodeDatasetName(){
    std::string line = decodeLine();
    std::vector<std::string> line_vector = StringUtils::splitByString(line, ",");
    std::string dataset_name = line_vector.at(1);
    std::vector<std::string> row = {"DATASET:", dataset_name};
    output_csv->writeRowDecoder(row);
    return dataset_name;
}

int HeaderDecoder::decodeDataRowsCount(){
    int bits = DatasetUtils::MAX_DATA_ROWS_BITS;
    int data_rows_count = input_file->getInt(bits);
    std::vector<std::string> row = {"DATA ROWS:", Conversor::intToString(data_rows_count)};
    output_csv->writeRowDecoder(row);
    return data_rows_count;
}

void HeaderDecoder::decodeFirstTimestamp(){
    long int seconds = input_file->getInt(32); // 32 bits for the timestamp
    std::string timestamp_str = HeaderTsUtils::getTimestampFromSeconds(seconds);
    std::vector<std::string> row = {"FIRST TIMESTAMP:", timestamp_str};
    output_csv->writeRowDecoder(row);
}

std::vector<Range*> HeaderDecoder::decodeMetadata(){
    std::string line = decodeLine();

    std::vector<Range*> ranges;
    while (line != DatasetUtils::DATA_HEADER){
        decodeMetadataRow(line, ranges);
        line = decodeLine();
    }
    std::vector<std::string> row = {DatasetUtils::DATA_HEADER};
    output_csv->writeRowDecoder(row);
    return ranges;
}

void HeaderDecoder::decodeMetadataRow(std::string line, std::vector<Range*> & ranges){
    std::vector<std::string> line_vector = StringUtils::splitByString(line, ",");
    output_csv->writeRowDecoder(line_vector);

    int minimum = Conversor::stringToInt(line_vector.at(3));
    int maximum = Conversor::stringToInt(line_vector.at(4));
    ranges.emplace_back(new Range(minimum, maximum));
}

int HeaderDecoder::decodeColumnNames(){
    std::string column_names = decodeLine();
    std::vector<std::string> row = StringUtils::splitByString(column_names, ",");
    output_csv->writeRowDecoder(row); // call writeRow instead of writeRowDecoder
    return row.size() - 1; // the -1 is due to the Time Delta column
}

std::string HeaderDecoder::decodeLine(){
    int number_of_chars = 0;
    // decode the number of chars in unary code
    while (input_file->getBit() > 0) { number_of_chars++; }
    int zeros_count = number_of_chars % 8 + 8;
    for(int i = 0; i < zeros_count - 1; i++) { input_file->getBit(); } // 0 bit
    // decode the chars (each char uses 1 byte)
    std::string line = "";
    for(int i = 0; i < number_of_chars; i++){
        int char_as_int = input_file->getInt(8);
        char character = Conversor::intToChar(char_as_int);
        line += character;
    }
    return line;
}