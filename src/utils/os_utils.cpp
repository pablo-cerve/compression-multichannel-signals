#include "os_utils.h"

#if defined(__unix__)
const std::string OSUtils::PROJECT_PATH = "/home/pablo/Documents/FING/Proyecto";
#else
const std::string OSUtils::PROJECT_PATH = "/Users/pablocerve/Documents/FING/Proyecto";
#endif
const std::string OSUtils::DATASETS_CSV_PATH = PROJECT_PATH + "/datasets-csv";
const std::string OSUtils::GIT_PATH = PROJECT_PATH + "/pc-tesis";
const std::string OSUtils::CPP_PROJECT_PATH = GIT_PATH + "/cpp_project";
