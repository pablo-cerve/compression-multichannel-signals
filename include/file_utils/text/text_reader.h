
#ifndef CPP_PROJECT_TEXT_READER_H
#define CPP_PROJECT_TEXT_READER_H

#include <fstream>
#include <string>


class TextReader {

protected:
    std::ifstream file;
    void readLineAux();
    void goToStart();

public:
    int total_lines;
    int current_line_count;
    std::string current_line;
    bool continue_reading;
    std::string full_path;

    TextReader();
    TextReader(std::string path, std::string filename);
    std::string readLine(); // PRE: continue_reading
    std::string readLineNoLastChar(); // PRE: continue_reading
    void goToLine(int line_number); // PRE: line_number <= total_lines
    bool findLine(std::string string_to_find);
    void close();
    ~TextReader();
};

#endif //CPP_PROJECT_TEXT_READER_H
