
#ifndef CPP_PROJECT_TESTS_EXAMPLES_H
#define CPP_PROJECT_TESTS_EXAMPLES_H

#include <iostream>
#include <vector>
#include "path.h"


class TestsExamples {
public:
    static void runAll(std::string full_dataset_path);
    TestsExamples(std::string full_dataset_path_);

private:
    std::string full_dataset_path;
    Path original_file_path;
    int window_size;
    std::vector<int> lossless_epsilons_vector;
    std::vector<int> lossy_epsilons_vector;

    void commonTest(std::string coder_name);
    void testLossless(std::string coder_name);
    void testLossy(std::string coder_name);

    static Path codedFilePath(std::string folder, Path file_path, std::string coder_name);
    static Path decodedFilePath(std::string folder, Path file_path, std::string coder_name);
    static void compareFiles(Path path1, Path path2);

};

#endif //CPP_PROJECT_TESTS_EXAMPLES_H
