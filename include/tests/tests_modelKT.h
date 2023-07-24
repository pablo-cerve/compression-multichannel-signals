
#ifndef CPP_PROJECT_TESTS_MODEL_KT_H
#define CPP_PROJECT_TESTS_MODEL_KT_H

#include "modelKT.h"

class TestsModelKT {
    typedef typename modelKT<int, 16, 14>::prob prob;

public:
    static void runAll();

private:
    static void getProbabilityTest();
    static void assertProb(prob p, int low, int high, int count);
};

#endif //CPP_PROJECT_TESTS_MODEL_KT_H
