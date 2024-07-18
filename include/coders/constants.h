
#ifndef CPP_PROJECT_CONSTANTS_H
#define CPP_PROJECT_CONSTANTS_H

#define COUT 0
#define CHECKS 0
#define RECORD_TESTS 0 // Set to 1 to set up the tests, then set to 0

#include <string>
#include <vector>

class Constants {

public:
    static const std::string NO_DATA;
    static const char NO_DATA_CHAR;
    static const double NO_DATA_DOUBLE;

    static bool isNoData(std::string csv_value);
    static bool isNoData(double value);

    static int getCoderValue(std::string coder_name);
    static std::string getCoderName(int coder_value);

    static bool isLosslessCoder(std::string coder_name);
    static bool requiresWindowSize(std::string coder_name);
};

#endif //CPP_PROJECT_CONSTANTS_H
