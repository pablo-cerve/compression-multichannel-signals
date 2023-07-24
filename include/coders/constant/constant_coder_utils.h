
#ifndef CPP_PROJECT_CONSTANT_CODER_UTILS_H
#define CPP_PROJECT_CONSTANT_CODER_UTILS_H

#include "conversor.h"
#include "math_utils.h"

class ConstantCoderUtils {

public:

    static std::string calculateConstantValue(double min, double max){
        double constant = min + max;
        if (constant != 0) { constant /= 2; }
        return Conversor::doubleToString(constant);
    }

    static bool validThreshold(double min, double max, double epsilon){
        double width = MathUtils::doubleAbsolute(max - min);
        return width <= 2*epsilon;
    }

};

#endif //CPP_PROJECT_CONSTANT_CODER_UTILS_H
