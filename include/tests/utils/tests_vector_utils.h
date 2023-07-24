
#ifndef CPP_PROJECT_TESTS_VECTOR_UTILS_H
#define CPP_PROJECT_TESTS_VECTOR_UTILS_H

#include "vector_utils.h"

class TestsVectorUtils {

public:
    static void runAll();

private:
    static void intVectorToStringVectorTest();
//    static void printIntVectorTest();
    static void vectorIncludesIntTest();
    static void removeOccurrencesTest();

};

#endif //CPP_PROJECT_TESTS_VECTOR_UTILS_H
