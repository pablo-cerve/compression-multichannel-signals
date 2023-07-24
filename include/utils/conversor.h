
#ifndef CPP_PROJECT_CONVERSOR_H
#define CPP_PROJECT_CONVERSOR_H

#include <string>

class Conversor {

public:
    static int charToInt(const char character);
    static const char intToChar(const int integer);

    static int doubleToInt(double d);

    static double stringToDouble(std::string & str);
    static int stringToInt(std::string & str);

    static std::string intToString(int & value);
    static std::string doubleToString(double d);
    static std::string doubleToIntToString(double d);


};

#endif //CPP_PROJECT_CONVERSOR_H
