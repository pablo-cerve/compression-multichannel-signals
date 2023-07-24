
#include "header_coder.h"

#include "assert.h"
#include "string_utils.h"
#include "datetime_utils.h"
#include "header_ts_utils.h"
#include "vector_utils.h"
#include "conversor.h"


HeaderCoder::HeaderCoder(CSVReader* input_csv_, BitStreamWriter* output_file_){
    test_mode = true;
    input_csv = input_csv_;
    output_file = output_file_;
}

HeaderCoder::HeaderCoder(CSVReader* input_csv_, CoderCommon* coder_common_){
    test_mode = false;
    input_csv = input_csv_;
    coder_common = coder_common_;
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
int HeaderCoder::codeHeader(Dataset* dataset){
    std::string dataset_name = codeDatasetName();
    dataset->setDatasetName(dataset_name);

    int data_rows_count = codeDataRowsCount();
    codeFirstTimestamp();
    std::string line;
    line = input_csv->readLineNoLastChar();
#if CHECKS
    assert(line == DatasetUtils::METADATA_HEADER);
#endif

    line = input_csv->readLineNoLastChar();
#if CHECKS
    assert(line == DatasetUtils::METADATA_COLUMNS);
#endif

    std::vector<Range*> ranges;
    std::vector<std::string> column_names;
    codeMetadata(column_names, ranges);

    int data_columns_count = codeColumnNames(column_names);
    dataset->setHeaderValues(ranges, data_columns_count);
    return data_rows_count;
}

std::string HeaderCoder::codeDatasetName(){
    // DATASET:,IRKIS
    std::string line = input_csv->readLine();
    std::vector<std::string> line_vector = CSVReader::split(line);
#if CHECKS
    checkSizeAndKey(line_vector, 2, "DATASET:");
#endif
    std::string dataset_name = line_vector[1];
    dataset_name = StringUtils::removeLastChar(dataset_name);
#if CHECKS
    assert(DatasetUtils::validDatasetName(dataset_name));
#endif
    codeLine(line);
    return dataset_name;
}

int HeaderCoder::codeDataRowsCount(){
    // DATA ROWS:,26305
    std::vector<std::string> line_vector = input_csv->readLineCSV();
#if CHECKS
    checkSizeAndKey(line_vector, 2, "DATA ROWS:");
#endif
    std::string data_rows_count_string = StringUtils::removeLastChar(line_vector[1]);
    int data_rows_count = Conversor::stringToInt(data_rows_count_string);
#if CHECKS
    assert(DatasetUtils::validDataRowsCount(data_rows_count));
#endif
    int bits = DatasetUtils::MAX_DATA_ROWS_BITS;
    test_mode ? output_file->pushInt(data_rows_count, bits) : coder_common->codeInt(data_rows_count, bits);
    return data_rows_count;
}

void HeaderCoder::codeFirstTimestamp(){
    // FIRST TIMESTAMP:|2017-01-01 00:00:00
    std::vector<std::string> line_vector = input_csv->readLineCSV();
#if CHECKS
    checkSizeAndKey(line_vector, 2, "FIRST TIMESTAMP:");
#endif
    std::string timestamp_str = StringUtils::removeLastChar(line_vector[1]);
    long int seconds = HeaderTsUtils::getSecondsFromTimestamp(timestamp_str);
    // 32 bits for the timestamp
    test_mode ? output_file->pushInt(seconds, 32) : coder_common->codeInt(seconds, 32);
}

void HeaderCoder::codeMetadata(std::vector<std::string> & column_names, std::vector<Range*> & ranges){
    int i = 0;
    std::string current_line = input_csv->readLine();
    while(StringUtils::removeLastChar(current_line) != DatasetUtils::DATA_HEADER) {
        codeMetadataRow(i, current_line, column_names, ranges);
        current_line = input_csv->readLine();
        i++;
    }
    codeLine(current_line);
}

void HeaderCoder::codeMetadataRow(int i,
                                  std::string line,
                                  std::vector<std::string> & column_names,
                                  std::vector<Range*> & ranges){
    // Time Delta,minutes,1,0,13071
    // -10cm_A,dimensionless,1000,0,600
    std::vector<std::string> line_vector = StringUtils::splitByChar(line, ','); // split by the comma
#if CHECKS
    assert(line_vector.size() == 5);
#endif

    std::string column_name = line_vector.at(0);
    column_names.emplace_back(column_name);
#if CHECKS
    if (i == 0){ assert(column_name == "Time Delta"); }

    std::string unit = line_vector.at(1);
    assert(DatasetUtils::validUnit(unit));

    std::string scale = line_vector.at(2);
    assert(DatasetUtils::validScale(scale));
#endif

    int minimum = Conversor::stringToInt(line_vector.at(3));
    int maximum = Conversor::stringToInt(line_vector.at(4));
    ranges.emplace_back(new Range(minimum, maximum));

    codeLine(line);
}

int HeaderCoder::codeColumnNames(std::vector<std::string> column_names){
    /*
    Time Delta,-10cm_A,-30cm_A,-50cm_A,-80cm_A,-120cm_A,-10cm_B,-30cm_B,-50cm_B,-80cm_B,-120cm_B
    0,N,N,N,N,N,N,N,N,N,N
    60,N,N,N,N,N,N,N,N,N,N
     . . .
    */
    std::string line = input_csv->readLine();
    std::vector<std::string> line_vector = CSVReader::split(line);
#if CHECKS
    int total_columns = line_vector.size();
    int total_metadata_columns = column_names.size();

    // VectorUtils::printStringVector(current_line);
    assert(total_columns >= total_metadata_columns);

    // The names must match
    for(int i=0; i < column_names.size(); i++){
        std::string column_name_1 = column_names.at(i);
        std::string column_name_2 = line_vector.at(i);
        if (i == line_vector.size() - 1){
            column_name_2 = StringUtils::removeLastChar(column_name_2);
        }
        assert(column_name_1 == column_name_2);
    }
    // the -1 is due to the Time Delta column
    assert((total_columns - 1) % (total_metadata_columns - 1) == 0);
#endif
    int data_columns_count = (int) line_vector.size() - 1;

    codeLine(line);
    return data_columns_count;
}

void HeaderCoder::codeLine(std::string line){
    int number_of_chars = (int) line.size() - 1;
    int zeros_count = number_of_chars % 8 + 8;

    // code the number of chars in unary code
    if (test_mode){
        output_file->pushBits(1, number_of_chars);
        output_file->pushBits(0, zeros_count);
    }
    else {
        coder_common->codeBits(1, number_of_chars);
        coder_common->codeBits(0, zeros_count);
    }
    // code the chars (each char uses 1 byte)
    for(int i=0; i < number_of_chars; i++) {
        char character = line[i];
        int char_as_int = Conversor::charToInt(character);
        test_mode ? output_file->pushInt(char_as_int, 8) : coder_common->codeInt(char_as_int, 8);
    }
}

void HeaderCoder::checkSizeAndKey(std::vector<std::string> line_vector, int expected_size, std::string expected_key){
    assert(line_vector.size() == expected_size);
    assert(line_vector[0] == expected_key);
}