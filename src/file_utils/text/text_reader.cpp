
#include "text_reader.h"

#include <iostream>
#include "assert.h"
#include "text_utils.h"
#include "string_utils.h"

TextReader::TextReader(){

}

TextReader::TextReader(std::string path, std::string filename){
    total_lines = TextUtils::lineCount(path, filename);
    full_path = path + "/" + filename;
    file.open(full_path);
    current_line_count = 0;
    continue_reading = total_lines > 0;
}

std::string TextReader::readLine(){
    assert(continue_reading);
    readLineAux();
    return current_line;
}

std::string TextReader::readLineNoLastChar(){
    readLine();
    return StringUtils::removeLastChar(current_line);
}

void TextReader::goToStart(){
    file.clear();
    file.seekg(0, std::ios::beg);
    current_line_count = 0;
    continue_reading = total_lines > 0;
}

void TextReader::goToLine(int line_number){
    assert(line_number <= total_lines);

    goToStart();
    for(int i=0; i < line_number; i++) { readLineAux(); }
}

bool TextReader::findLine(const std::string string_to_find){
    while (continue_reading) {
        readLineAux();
        if (StringUtils::find(current_line, string_to_find)) { return true; }
    }
    return false;
}

void TextReader::readLineAux() {
    std::getline(file, current_line);
    current_line_count++;
    continue_reading = current_line_count < total_lines;
}

void TextReader::close(){
    file.close();
}

TextReader::~TextReader(){
    close();
}
