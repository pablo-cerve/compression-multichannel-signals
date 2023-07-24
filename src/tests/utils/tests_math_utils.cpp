
#include "tests_math_utils.h"
#include "math_utils.h"
#include "conversor.h"
#include <cassert>
#include <iostream>
#include <math.h>

void TestsMathUtils::runAll(){
    std::cout << "  bitLengthTest();" << std::endl;           bitLengthTest();
    std::cout << "  windowSizeBitLengthTest();" << std::endl; windowSizeBitLengthTest();
    std::cout << "  doubleToIntTest();" << std::endl;         doubleToIntTest();
    std::cout << "  intAbsoluteTest();" << std::endl;         intAbsoluteTest();
    std::cout << "  doubleAbsoluteTest();" << std::endl;      doubleAbsoluteTest();
    std::cout << "  halfTest();" << std::endl;                halfTest();
}

void TestsMathUtils::bitLengthTest(){
    assert(MathUtils::bitLength(0) == 1); // 0
    assert(MathUtils::bitLength(1) == 1); // 1
    assert(MathUtils::bitLength(2) == 2); // 10
    assert(MathUtils::bitLength(3) == 2); // 11
    assert(MathUtils::bitLength(4) == 3); // 100
    assert(MathUtils::bitLength(5) == 3); // 101
    assert(MathUtils::bitLength(6) == 3); // 110
    assert(MathUtils::bitLength(7) == 3); // 111
    assert(MathUtils::bitLength(8) == 4); // 1000
    assert(MathUtils::bitLength(pow(2,10) - 1) == 10);
    assert(MathUtils::bitLength(pow(2,10)) == 11);
    assert(MathUtils::bitLength(pow(2,10) + 1) == 11);
}

void TestsMathUtils::windowSizeBitLengthTest(){
    assert(MathUtils::windowSizeBitLength(2) == 1);
    assert(MathUtils::windowSizeBitLength(3) == 2);
    assert(MathUtils::windowSizeBitLength(4) == 2);
    assert(MathUtils::windowSizeBitLength(5) == 3);
    assert(MathUtils::windowSizeBitLength(6) == 3);
    assert(MathUtils::windowSizeBitLength(7) == 3);
    assert(MathUtils::windowSizeBitLength(8) == 3);
    assert(MathUtils::windowSizeBitLength(9) == 4);
    assert(MathUtils::windowSizeBitLength(pow(2,10) - 1) == 10);
    assert(MathUtils::windowSizeBitLength(pow(2,10)) == 10);
    assert(MathUtils::windowSizeBitLength(pow(2,10) + 1) == 11);
}

void TestsMathUtils::doubleToIntTest(){
    double double1, double2, double3;
    double1 = -50; double2 = 0; double3 = 50;
    assert(Conversor::doubleToInt(double1) == -50);
    assert(Conversor::doubleToInt(double2) == 0);
    assert(Conversor::doubleToInt(double3) == 50);
}

void TestsMathUtils::intAbsoluteTest(){
    assert(MathUtils::intAbsolute(-50) == 50);
    assert(MathUtils::intAbsolute(0) == 0);
    assert(MathUtils::intAbsolute(50) == 50);
}

void TestsMathUtils::doubleAbsoluteTest(){
    assert(MathUtils::doubleAbsolute(-50.8) == 50.8);
    assert(MathUtils::doubleAbsolute(0) == 0);
    assert(MathUtils::doubleAbsolute(50.8) == 50.8);
}

void TestsMathUtils::halfTest(){
    assert(MathUtils::half(0, 0) == 0);
    assert(MathUtils::half(0, 1) == 0);
    assert(MathUtils::half(1, 1) == 1);
    assert(MathUtils::half(0, 2) == 1);
}
