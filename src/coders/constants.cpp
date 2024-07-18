#include "constants.h"

#include <iostream>
#include "string_utils.h"
#include "vector_utils.h"

const std::string Constants::NO_DATA = "N";
const char Constants::NO_DATA_CHAR = 'N';
const double Constants::NO_DATA_DOUBLE = 0;

bool Constants::isNoData(std::string csv_value) {
    return csv_value[0] == NO_DATA_CHAR;
}

bool Constants::isNoData(double value) {
    return value == NO_DATA_DOUBLE;
}

int Constants::getCoderValue(std::string coder_name) {
    // lossless
    if      (coder_name == "Base")     { return 0; }
    else if (coder_name == "Gorilla")  { return 1; }
    else if (coder_name == "TS2Diff")  { return 2; }
    // lossy
    else if (coder_name == "PCA*")     { return 10; }
    else if (coder_name == "APCA*")    { return 11; }
    else if (coder_name == "PWLH*")    { return 20; }
    else if (coder_name == "PWLH*Int") { return 21; }
    else if (coder_name == "CA*")      { return 22; }
    else if (coder_name == "SF*")      { return 23; }
    else if (coder_name == "FR*")      { return 24; }
    else if (coder_name == "GAMPS*")   { return 30; }

    throw std::invalid_argument("Invalid coder name: \"" + coder_name + "\".");
}

std::string Constants::getCoderName(int coder_value) {
    // lossless
    if      (coder_value == 0)  { return "Base"; }
    else if (coder_value == 1)  { return "Gorilla"; }
    else if (coder_value == 2)  { return "TS2Diff"; }
    // lossy
    else if (coder_value == 10) { return "PCA*"; }
    else if (coder_value == 11) { return "APCA*"; }
    else if (coder_value == 20) { return "PWLH*"; }
    else if (coder_value == 21) { return "PWLH*Int"; }
    else if (coder_value == 22) { return "CA*"; }
    else if (coder_value == 23) { return "SF*"; }
    else if (coder_value == 24) { return "FR*"; }
    else if (coder_value == 30) { return "GAMPS*"; }

    throw std::invalid_argument("Invalid coder value: " + std::to_string(coder_value) + ".");
}

bool Constants::isLosslessCoder(std::string coder_name) {
    std::vector<std::string> lossless_coders = {"Base", "Gorilla", "TS2Diff"};
    return VectorUtils::vectorIncludesString(lossless_coders, coder_name);
}

bool Constants::requiresWindowSize(std::string coder_name) {
    std::vector<std::string> coders_without_window = {"Base", "Gorilla"};
    return !VectorUtils::vectorIncludesString(coders_without_window, coder_name);
}
