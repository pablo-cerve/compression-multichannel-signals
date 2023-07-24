
#ifndef CPP_PROJECT_TESTS_CODERS_UTILS_H
#define CPP_PROJECT_TESTS_CODERS_UTILS_H

#include "iostream"
#include "csv_writer.h"
#include "dataset.h"

class TestsCodersUtils {
public:
    static std::string setAndWriteCoderName(std::string coder_name, CSVWriter* csv_writer);
    static void writeBitsCSV(CSVWriter* csv_writer, Dataset* dataset);
    static void writeStringCSV(CSVWriter* csv_writer, std::string mode, bool title);
    static Path codedFilePath(std::string folder, Path file_path, std::string coder_name);
    static Path decodedFilePath(std::string folder, Path file_path);
    static void compareDecodedFiles(std::string mode, Path file_path, Path output_decode_path, std::string expected_path_str, std::string coder_name);
    static void compareFiles(Path path1, Path path2);
};

#endif //CPP_PROJECT_TESTS_CODERS_UTILS_H
