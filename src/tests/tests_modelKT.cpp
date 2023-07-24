
#include "tests_modelKT.h"
#include "assert.h"
#include <iostream>
#include "model_metrics.h"

void TestsModelKT::runAll(){
    std::cout << "  getProbabilityTest()" << std::endl; getProbabilityTest();
}

void TestsModelKT::getProbabilityTest(){
    modelKT<int, 16, 14> model;
    prob p;
    // state 1 (no_data = true)
    p = model.getProbability(0); assertProb(p, 0, 1, 2);

    // state 0 (no_data = false)
    p = model.getProbability(0); assertProb(p, 0, 1, 2);
    p = model.getProbability(0); assertProb(p, 0, 3, 4);
    p = model.getProbability(1); assertProb(p, 5, 6, 6);

    // state 1 (no_data = true)
    p = model.getProbability(1); assertProb(p, 3, 4, 4);
    p = model.getProbability(1); assertProb(p, 3, 6, 6);
    p = model.getProbability(0); assertProb(p, 0, 3, 8);
};

void TestsModelKT::assertProb(prob p, int low, int high, int count){
    // std::cout << low << " " << high << " " << count << std::endl;
    // std::cout << p.low << " " << p.high << " " << p.count << std::endl;
    assert(p.low == low);
    assert(p.high == high);
    assert(p.count == count);
}
