
#ifndef CPP_PROJECT_TESTS_H
#define CPP_PROJECT_TESTS_H

#include <string>
#include "dataset.h"

class Tests {

public:
    static void runAll(std::string full_dataset_path);
private:
    static bool printError(std::exception e);
};

#endif //CPP_PROJECT_TESTS_H
