
#include "main_helper.h"

#include <iostream>
#include "decoder_base.h"
#include "decoder_ts2diff.h"
#include "decoder_gorilla.h"
#include "decoder_pca.h"
#include "decoder_apca.h"
#include "decoder_pwlh.h"
#include "decoder_ca.h"
#include "decoder_slide_filter.h"
#include "decoder_fr.h"
#include "decoder_gamps.h"
#include "coder_base.h"
#include "coder_ts2diff.h"
#include "coder_gorilla.h"
#include "coder_pca.h"
#include "coder_apca.h"
#include "coder_pwlh.h"
#include "coder_ca.h"
#include "coder_slide_filter.h"
#include "coder_fr.h"
#include "coder_gamps.h"
#include "csv_utils.h"
#include "bit_stream_utils.h"
#include "assert.h"
#include "string_utils.h"
#include "dataset_utils.h"
#include "epsilon_mapper.h"


void MainHelper::checkAction(std::string action){
    if (action == "test" || action == "code" || action == "decode") { return; }

    std::string err_msg = "The first argument must be \"code\", \"decode\" or \"test\", it cannot be \"" + action + "\".";
    throw std::invalid_argument(err_msg);
}

bool MainHelper::parseErrorMode(std::string error_mode){
    if (error_mode == "error_mode=epsilon"){ return true;  }
    if (error_mode == "error_mode=e")      { return false; }

    std::string err_msg = "The error mode argument must be \"error_mode=epsilon\" or \"error_mode=e\", it cannot be \"" + error_mode + "\".";
    throw std::invalid_argument(err_msg);
}

void MainHelper::checkEpsilonsVectorLength(std::vector<int> epsilons_vector, std::string full_input_path){
    Path path = Path(full_input_path);
    std::string dataset_name = DatasetUtils::getDatasetNameFromFilePath(path.file_path);

    if (dataset_name == "invalid"){
        return; // the input file is probably a custom file, skip check
    }

    // the input file is inside an original dataset folder
    std::vector<int> other_epsilons_vector;
    std::string dataset_filename = path.file_filename;
    try {
        // error_param value doesn't matter, what matters is not the content of other_epsilons_vector but its size
        other_epsilons_vector = EpsilonMapper::getEpsilonsVector(dataset_name, dataset_filename, 0);
    }
    catch(...) {
        return; // the input filename doesn't match any other filename in the dataset, skip check
    }

    // the input filename matches another filename in the dataset, check that the vector sizes match
    int actual_size = epsilons_vector.size();
    int expected_size = other_epsilons_vector.size();
    if (actual_size != expected_size){
        throw std::invalid_argument("The number of epsilons provided (" + std::to_string(actual_size) + ") is different than the number expected (" + std::to_string(expected_size) + ").");
    }
}

std::vector<int> MainHelper::getEpsilonsVector(int error_param, std::string full_input_path){
    Path path = Path(full_input_path);
    std::string dataset_name = DatasetUtils::getDatasetNameFromFilePath(path.file_path);

    if (dataset_name == "invalid"){
        throw std::invalid_argument("In order to run the coder in error_mode=e, the input csv must be inside the original folder, e.g. \"[1]IRKIS\", \"[2]NOAA-SST\", etc.");
    }

    std::string dataset_filename = path.file_filename;
    std::vector<int> epsilons_vector = EpsilonMapper::getEpsilonsVector(dataset_name, dataset_filename, error_param);
    return removeNegativeNumbers(epsilons_vector); // change -1 values by 0
}

Dataset* MainHelper::code(std::string coder_name, std::string full_input_path, std::string full_output_path,
                          int window_size, std::vector<int> epsilons_vector){
    CSVReader* csv_reader = getCSVReader(full_input_path);
    BitStreamWriter* bit_stream_writer = getBitStreamWriter(full_output_path);

    epsilons_vector.insert(epsilons_vector.begin(), 0); // add epsilon for timestamp column

    CoderCommon* coder;

    // lossless without window param
    if (coder_name == "Base"){
        coder = new CoderBase(coder_name, csv_reader, bit_stream_writer);
    }
    else if (coder_name == "Gorilla"){
        coder = new CoderGorilla(coder_name, csv_reader, bit_stream_writer);
    }
    // lossless with window param
    else if (coder_name == "TS2Diff"){
        coder = new CoderTS2Diff(coder_name, csv_reader, bit_stream_writer);
        ((CoderTS2Diff*) coder)->setWindowSize(window_size);
    }
    // lossy with window param
    else if (coder_name == "PCA*"){
        coder = new CoderPCA(coder_name, csv_reader, bit_stream_writer);
        ((CoderPCA*) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else if (coder_name == "APCA*"){
        bool mask_mode = true;
        coder = new CoderAPCA(coder_name, csv_reader, bit_stream_writer);
        ((CoderAPCA*) coder)->setCoderParams(window_size, epsilons_vector, mask_mode);
    }
    else if (coder_name == "CA*"){
        coder = new CoderCA(coder_name, csv_reader, bit_stream_writer);
        ((CoderCA*) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else if (coder_name == "PWLH*" || coder_name == "PWLH*Int"){
        coder = new CoderPWLH(coder_name, csv_reader, bit_stream_writer);
        ((CoderPWLH*) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else if (coder_name == "SF*"){
        coder = new CoderSlideFilter(coder_name, csv_reader, bit_stream_writer);
        ((CoderSlideFilter*) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else if (coder_name == "FR*"){
        coder = new CoderFR(coder_name, csv_reader, bit_stream_writer);
        ((CoderFR*) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else if (coder_name == "GAMPS*") {
        coder = new CoderGAMPS(coder_name, csv_reader, bit_stream_writer);
        ((CoderGAMPS *) coder)->setCoderParams(window_size, epsilons_vector);
    }
    else {
        throw std::invalid_argument("Invalid coder: " + coder_name);
    }

    coder->codeCoderName();
    if (Constants::requiresWindowSize(coder_name)){
        coder->codeWindowParameter();
    }
    return coder->code();
}

void MainHelper::decode(std::string full_input_path, std::string full_output_path){
    BitStreamReader* bit_stream_reader = getBitStreamReader(full_input_path);
    CSVWriter* csv_writer = getCSVWriter(full_output_path);
    DecoderCommon* decoder;

    std::string coder_name = DecoderCommon::decodeCoderName(bit_stream_reader);

    if (coder_name == "Base") {
        decoder = new DecoderBase(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "Gorilla") {
        decoder = new DecoderGorilla(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "TS2Diff"){
        decoder = new DecoderTS2Diff(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "PCA*") {
        decoder = new DecoderPCA(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "APCA*") {
        decoder = new DecoderAPCA(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "PWLH*" || coder_name == "PWLH*Int") {
        decoder = new DecoderPWLH(coder_name, bit_stream_reader, csv_writer);
        ((DecoderPWLH *) decoder)->setIntegerMode();
    }
    else if (coder_name == "CA*") {
        decoder = new DecoderCA(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "FR*") {
        decoder = new DecoderFR(coder_name, bit_stream_reader, csv_writer);
    }
    else if (coder_name == "SF*") {
        decoder = new DecoderSlideFilter(coder_name, bit_stream_reader, csv_writer);
    }
    else { // if (coder_name == "GAMPS*"){
        assert(coder_name == "GAMPS*");
        decoder = new DecoderGAMPS(coder_name, bit_stream_reader, csv_writer);
    }

    if (Constants::requiresWindowSize(coder_name)){
        decoder->decodeWindowParameter();
    }
    decoder->decode();
}

CSVReader* MainHelper::getCSVReader(std::string full_path){
    try {
        Path path = Path(full_path);
        CSVReader* csv_reader = new CSVReader(full_path);
        return csv_reader;
    }
    catch(...) {
        throw std::invalid_argument("The input csv file \"" + full_path + "\" could not be opened.");
    }
}

CSVWriter* MainHelper::getCSVWriter(std::string full_path){
    try {
        Path path = Path(full_path);
        CSVWriter* csv_writer= new CSVWriter(full_path);
        return csv_writer;
    }
    catch(...) {
        throw std::invalid_argument("The output csv file \"" + full_path + "\" could not be opened.");
    }
}

BitStreamReader* MainHelper::getBitStreamReader(std::string full_path){
    try {
        Path path = Path(full_path);
        BitStreamReader* bit_stream_reader = new BitStreamReader(full_path);
        return bit_stream_reader;
    }
    catch(...) {
        throw std::invalid_argument("The input file \"" + full_path + "\" could not be opened.");
    }
}

BitStreamWriter* MainHelper::getBitStreamWriter(std::string full_path){
    try {
        Path path = Path(full_path);
        BitStreamWriter* bit_stream_writer = new BitStreamWriter(full_path);
        return bit_stream_writer;
    }
    catch(...) {
        throw std::invalid_argument("The output file \"" + full_path + "\" could not be opened.");
    }
}

std::vector<int> MainHelper::removeNegativeNumbers(std::vector<int> epsilons_vector){
    std::vector<int> result;
    for(int i=0; i < epsilons_vector.size(); i++){
        int element = epsilons_vector.at(i);
        if (element != -1) {
            result.push_back(element);
        }
        else {
            result.push_back(0);
        }
    }
    return result;
}
