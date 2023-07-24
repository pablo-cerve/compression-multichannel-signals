
#include "csv_reader.h"

#include <iostream>
#include <vector>
#include "string_utils.h"
#include "vector_utils.h"
#include "text_utils.h"

void CSVReader::constructor(std::string path, std::string filename){
    full_path = path + "/" + filename;
    file.open(full_path);
    if (!file.is_open()){
        throw std::runtime_error("Error opening csv file.");
    }
    current_line_count = 0;
    total_lines = TextUtils::lineCount(path, filename);
    continue_reading = total_lines > 0;
    first_data_row = 0;
}

CSVReader::CSVReader(Path path){
    constructor(path.file_path, path.file_filename);
}

std::vector<std::string> CSVReader::split(std::string line){
    return StringUtils::splitByChar(line, ','); // split by the comma
}

std::vector<std::string> CSVReader::readLineCSV(){
    std::string current_line = readLine();
    std::vector<std::string> current_line_vector = split(current_line);
    return current_line_vector;
}

std::string CSVReader::readNextValue(){
    return readNextValue(column_index);
};

std::string CSVReader::readNextValue(int col_index){
    std::string current_line = readLine();
    std::string col_value = StringUtils::splitByCharWithIndex(current_line, ',', col_index);
    return col_value;
};

void CSVReader::goToFirstDataRow(int column_index_){
    column_index = column_index_;
    if (first_data_row == 0) {
        int current_line_count = 7;
        goToLine(current_line_count); // "DATA:" is in line 7 when there is only one column in the metadata
        while (continue_reading){
            std::string csv_value = StringUtils::removeLastChar(readNextValue(0));
            if (csv_value == "DATA:") {
                first_data_row = current_line_count + 2; // +2 to skip the row with the column names
                break;
            }
            current_line_count += 1;
        }
    }
    goToLine(first_data_row);
}
