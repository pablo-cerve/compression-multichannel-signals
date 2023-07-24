
#include "tests_examples.h"
//#include "assert.h"
#include "tests_utils.h"
#include "path.h"
#include "main_helper.h"
#include "tests_coders_utils.h"
#include "bit_stream_utils.h"

void TestsExamples::runAll(){
    commonTest("PCA*",     4);
    commonTest("APCA*",    256);
    commonTest("GAMPS*",   256);
    commonTest("CA*",      256);
    commonTest("PWLH*",    256);
    commonTest("PWLH*Int", 256);
    commonTest("SF*",      256);
    commonTest("FR*",      256);
}

void TestsExamples::commonTest(std::string coder_name, int window_size){
    std::cout << "  commonTest(" + coder_name + ") - lossy" << std::endl;
    testLossy("example.csv", coder_name, window_size);

    std::cout << "  commonTest(" + coder_name + ") - lossless" << std::endl;
    testLossless("example.csv", coder_name, window_size);
}

void TestsExamples::testLossy(std::string input_filename, std::string coder_name, int window_size){
    std::vector<int> epsilons_vector{1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    Path input_path = Path(EXAMPLES_PATH, input_filename);
    Path coded_path = TestsCodersUtils::codedFilePath(OUTPUT_PATH, input_path, coder_name);

    Path expected_decoded_path = TestsCodersUtils::decodedFilePath(EXPECTED_PATH, coded_path);
#if RECORD_TESTS
    Path decoded_path = expected_decoded_path;
#else
    Path decoded_path = TestsCodersUtils::decodedFilePath(OUTPUT_PATH, coded_path);
#endif // RECORD_TESTS

    MainHelper::code(coder_name, input_path.full_path, coded_path.full_path, window_size, epsilons_vector);
    MainHelper::decode(coded_path.full_path, decoded_path.full_path);
    BitStreamUtils::removeFile(coded_path);

#if !RECORD_TESTS
    TestsCodersUtils::compareFiles(decoded_path, expected_decoded_path);
    BitStreamUtils::removeFile(decoded_path);
#endif // !RECORD_TESTS
}

void TestsExamples::testLossless(std::string input_filename, std::string coder_name, int window_size){
    std::vector<int> epsilons_vector{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Path input_path = Path(EXAMPLES_PATH, input_filename);
    Path coded_path = TestsCodersUtils::codedFilePath(OUTPUT_PATH, input_path, coder_name);

    Path expected_decoded_path = TestsCodersUtils::decodedFilePath(EXPECTED_PATH, coded_path);
    Path decoded_path = TestsCodersUtils::decodedFilePath(OUTPUT_PATH, coded_path);

    MainHelper::code(coder_name, input_path.full_path, coded_path.full_path, window_size, epsilons_vector);
    MainHelper::decode(coded_path.full_path, decoded_path.full_path);
    BitStreamUtils::removeFile(coded_path);

    TestsCodersUtils::compareFiles(decoded_path, input_path); // the decoded file should be the same as the original one
    BitStreamUtils::removeFile(decoded_path);
}

const std::string TestsExamples::EXAMPLES_PATH = TestsUtils::OUTPUT_PATH + "/examples";
const std::string TestsExamples::EXPECTED_PATH = EXAMPLES_PATH + "/expected";
const std::string TestsExamples::OUTPUT_PATH   = EXAMPLES_PATH + "/output";
