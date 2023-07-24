
#include "tests_coders_utils.h"
#include "vector_utils.h"
#include "bit_stream_utils.h"
#include "csv_utils.h"
#include "assert.h"
#include "string_utils.h"


std::string TestsCodersUtils::setAndWriteCoderName(std::string coder_name, CSVWriter* csv_writer){
    std::cout << ">> " << coder_name << std::endl;
    csv_writer->writeRow(coder_name);
    return coder_name;
}

void TestsCodersUtils::writeBitsCSV(CSVWriter* csv_writer, Dataset* dataset){
    csv_writer->writeRow("TOTAL  = " + Conversor::intToString(dataset->total_bits));
    csv_writer->writeRow("header = " + Conversor::intToString(dataset->header_bits));
    csv_writer->writeRow("mask   = " + VectorUtils::intVectorToString(dataset->totalMaskBitsArray()));
    csv_writer->writeRow("data   = " + VectorUtils::intVectorToString(dataset->totalBitsArray()));
}

void TestsCodersUtils::writeStringCSV(CSVWriter* csv_writer, std::string mode, bool title){
    std::cout << ">> " << mode << std::endl;
    if (title){
        csv_writer->writeRow("");
    }
    csv_writer->writeRow("");
    csv_writer->writeRow(mode);
    csv_writer->writeRow("");
}

Path TestsCodersUtils::codedFilePath(std::string folder, Path file_path, std::string coder_name){
    return Path(folder, file_path.file_filename + "-" + coder_name + "-C.bin");
}

Path TestsCodersUtils::decodedFilePath(std::string folder, Path file_path){
    return Path(folder, file_path.file_filename + "-D.csv");
}

void TestsCodersUtils::compareDecodedFiles(std::string mode, Path file_path, Path output_decode_path, std::string expected_path_str, std::string coder_name){
    if (mode == "LOSSLESS"){
        // original_file should be the same as decoded(coded(original_file))
        compareFiles(file_path, output_decode_path);
        BitStreamUtils::removeFile(output_decode_path);
    }
    else {
        Path expected_decode_path = decodedFilePath(expected_path_str, file_path);
        compareFiles(expected_decode_path, output_decode_path);
    }
}

void TestsCodersUtils::compareFiles(Path path1, Path path2){
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
