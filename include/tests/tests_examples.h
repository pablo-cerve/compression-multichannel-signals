
#ifndef CPP_PROJECT_TESTS_EXAMPLES_H
#define CPP_PROJECT_TESTS_EXAMPLES_H

#include <iostream>
#include <vector>


class TestsExamples {
public:
    static void runAll();

private:
    static void commonTest(std::string coder_name, int window_size);
    static void testLossy(std::string input_filename, std::string coder_name, int window_size);
    static void testLossless(std::string input_filename, std::string coder_name, int window_size);

    static const std::string EXAMPLES_PATH;
    static const std::string EXPECTED_PATH;
    static const std::string OUTPUT_PATH;

};

#endif //CPP_PROJECT_TESTS_EXAMPLES_H
