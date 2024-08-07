
#include "tests_examples.h"
#include "constants.h"
#include "main_helper.h"
#include "bit_stream_utils.h"
#include "string_utils.h"
#include "assert.h"
#include "csv_utils.h"

void TestsExamples::runAll(std::string full_dataset_path){
    TestsExamples* test_examples = new TestsExamples(full_dataset_path);
    test_examples->commonTest("Base");
    test_examples->commonTest("Gorilla");
    test_examples->commonTest("TS2Diff");
    test_examples->commonTest("PCA*");
    test_examples->commonTest("APCA*");
    test_examples->commonTest("CA*");
    test_examples->commonTest("PWLH*");
    test_examples->commonTest("PWLH*Int");
    test_examples->commonTest("SF*");
    test_examples->commonTest("FR*");
    test_examples->commonTest("GAMPS*");
}

TestsExamples::TestsExamples(std::string full_dataset_path_){
    full_dataset_path = full_dataset_path_;
    original_file_path = Path(full_dataset_path + "/[1]IRKIS", "vwc_1202.dat.csv");
    window_size = 4;
    lossless_epsilons_vector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    lossy_epsilons_vector = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
}

void TestsExamples::commonTest(std::string coder_name){
    bool is_lossless = Constants::isLosslessCoder(coder_name);

    if (is_lossless){
        std::cout << "  Testing Coder " << coder_name << std::endl;
        testLossless(coder_name);
        return;
    }

    std::cout << "  Testing Coder " << coder_name << " - lossless case" << std::endl;
    testLossless(coder_name);
    std::cout << "  Testing Coder " << coder_name << " - lossy case" << std::endl;
    testLossy(coder_name);
}

void TestsExamples::testLossless(std::string coder_name){
    Path coded_path = codedFilePath(full_dataset_path, original_file_path, coder_name);
    Path decoded_path = decodedFilePath(full_dataset_path, original_file_path, coder_name);

    MainHelper::code(coder_name, original_file_path.full_path, coded_path.full_path, window_size, lossless_epsilons_vector);
    MainHelper::decode(coded_path.full_path, decoded_path.full_path);

    compareFiles(original_file_path, decoded_path);

    BitStreamUtils::removeFile(coded_path);
    BitStreamUtils::removeFile(decoded_path);
}

void TestsExamples::testLossy(std::string coder_name){
    Path coded_path = codedFilePath(full_dataset_path, original_file_path, coder_name);
    Path decoded_path = decodedFilePath(full_dataset_path, original_file_path, coder_name);

    MainHelper::code(coder_name, original_file_path.full_path, coded_path.full_path, window_size, lossy_epsilons_vector);
    MainHelper::decode(coded_path.full_path, decoded_path.full_path);

    BitStreamUtils::removeFile(coded_path);
    BitStreamUtils::removeFile(decoded_path);
}

Path TestsExamples::codedFilePath(std::string folder, Path file_path, std::string coder_name){
    return Path(folder, file_path.file_filename + "-" + coder_name + "-coded.bin");
}

Path TestsExamples::decodedFilePath(std::string folder, Path file_path, std::string coder_name){
    return Path(folder, file_path.file_filename + "-" + coder_name + "-decoded.csv");
}

void TestsExamples::compareFiles(Path path1, Path path2){
    int res = BitStreamUtils::compareBytes(path1, path2);
    if (res == 0) { return; }

    std::cout << "FAILURE!" << std::endl;
    std::cout << "File 1 = " << path1.full_path << std::endl;
    std::cout << "File 2 = " << path2.full_path << std::endl;
    std::cout << "First diff byte = " << res << std::endl;

    std::vector<std::string> filename1_split = StringUtils::splitByChar(path1.file_filename, '.');
    std::vector<std::string> filename2_split = StringUtils::splitByChar(path2.file_filename, '.');
    std::string file1_ext = filename1_split[filename1_split.size()-1];
    std::string file2_ext = filename2_split[filename2_split.size()-1];

    if (file1_ext == "csv" && file2_ext == "csv"){
        std::cout << "Compare CSV..." << std::endl;
        CSVUtils::CompareCSVLossless(path1, path2);
    }
    assert(res == 0);
}
