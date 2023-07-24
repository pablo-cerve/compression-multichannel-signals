
#ifndef CPP_PROJECT_CSV_WRITER_H
#define CPP_PROJECT_CSV_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include "path.h"

class CSVWriter {

private:
    std::ofstream file;

    void constructor(std::string path, std::string filename);

public:
    std::string full_path;
    int current_line_count = 0;

    CSVWriter(std::string path, std::string filename);
    CSVWriter(Path path);

    void writeRow(std::vector<std::string> row);
    void writeRow(std::string str);
    void writeRowDecoder(std::vector<std::string> row);
    void close();
};

#endif //CPP_PROJECT_CSV_WRITER_H
