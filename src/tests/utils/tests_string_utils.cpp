
#include "tests_string_utils.h"
#include "string_utils.h"
#include "conversor.h"
#include <cassert>
#include <iostream>

void TestsStringUtils::runAll(){
    std::cout << "  splitByCharWithIndexTest();" << std::endl; splitByCharWithIndexTest();
    std::cout << "  charToIntTest();" << std::endl;            charToIntTest();
    std::cout << "  intToCharTest();" << std::endl;            intToCharTest();
}

void TestsStringUtils::splitByCharWithIndexTest(){

    std::string line = "60,N,N,N,N,N,N,N,N,N,N\r";
    assert(StringUtils::splitByCharWithIndex(line, ',', 0) == "60");
    assert(StringUtils::splitByCharWithIndex(line, ',', 1) == "N");
    assert(StringUtils::splitByCharWithIndex(line, ',', 9) == "N");
    assert(StringUtils::splitByCharWithIndex(line, ',', 10) == "N\r");
    assert(StringUtils::splitByCharWithIndex(line, ',', 11) == "");
}

void TestsStringUtils::charToIntTest(){
    assert(Conversor::charToInt('A') == 65);
    assert(Conversor::charToInt('Z') == 90);
    assert(Conversor::charToInt('a') == 97);
    assert(Conversor::charToInt('z') == 122);
}

void TestsStringUtils::intToCharTest(){
    assert(Conversor::intToChar(65) == 'A');
    assert(Conversor::intToChar(90) == 'Z');
    assert(Conversor::intToChar(97) == 'a');
    assert(Conversor::intToChar(122) == 'z');
}
