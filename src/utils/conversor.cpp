
#include "conversor.h"
#include <cmath>

int Conversor::charToInt(const char character){
    return (int) character;
}

const char Conversor::intToChar(const int integer){
    return (char) integer;
}

int Conversor::doubleToInt(double d){
    int d_int = (int)round(d);
    return d_int;
}

double Conversor::stringToDouble(std::string & str){
    const char* c = str.c_str();
    return atof(c);
}

int Conversor::stringToInt(std::string & str){
    return std::stoi(str);
}

std::string Conversor::intToString(int & value){
    return std::to_string(value);
}

std::string Conversor::doubleToString(double d){
    std::string str = std::to_string(d);
    return str;
}

std::string Conversor::doubleToIntToString(double d){
    int d_int = Conversor::doubleToInt(d);
    std::string str = Conversor::intToString(d_int);
    return str;
}
