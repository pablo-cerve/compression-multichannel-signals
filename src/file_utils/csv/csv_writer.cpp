
#include "csv_writer.h"
#include "string_utils.h"

#include <iostream>

void CSVWriter::constructor(std::string path, std::string filename){
    full_path = path + "/" + filename;
    file.open(full_path);
    if (!file.is_open()){
        throw std::runtime_error("Error creating csv file.");
    }
}

CSVWriter::CSVWriter(std::string path, std::string filename){
    constructor(path, filename);
}

CSVWriter::CSVWriter(Path path){
    constructor(path.file_path, path.file_filename);
}

void CSVWriter::writeRow(std::vector<std::string> row){
    current_line_count++;
    std::string line = StringUtils::join(row, ",");
    file << line;
    file << '\n'; // LF
}

void CSVWriter::writeRow(std::string str){
    std::vector<std::string> row{str};
    writeRow(row);
}

void CSVWriter::writeRowDecoder(std::vector<std::string> row){
    current_line_count++;
    std::string line = StringUtils::join(row, ",");
    file << line;
    file << (unsigned char) 0xD; // CR
    file << '\n'; // LF
}

void CSVWriter::close(){
    file.close();
}
