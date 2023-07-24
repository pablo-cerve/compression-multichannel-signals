
#include "tests_vector_utils.h"
#include "assert.h"

void TestsVectorUtils::runAll(){
    std::cout << "  intVectorToStringVectorTest()" << std::endl; intVectorToStringVectorTest();
    std::cout << "  vectorIncludesIntTest()" << std::endl;       vectorIncludesIntTest();
    std::cout << "  removeOccurrencesTest()" << std::endl;       removeOccurrencesTest();
}

void TestsVectorUtils::intVectorToStringVectorTest(){

    std::vector<int> vector{1, 2, 3, 4, 5};
    std::vector<std::string> expected_vector{"1", "2", "3", "4", "5"};
    assert(VectorUtils::intVectorToStringVector(vector) == expected_vector);

}
void TestsVectorUtils::vectorIncludesIntTest(){
    std::vector<int> vector1{1, 2, 3, 4, 5};
    std::vector<int> vector2{-1, 1, 2, -1, 3, 4, 5, -1};
    assert(VectorUtils::vectorIncludesInt(vector1, -1) == false);
    assert(VectorUtils::vectorIncludesInt(vector2, -1) == true);
}

void TestsVectorUtils::removeOccurrencesTest(){
    std::vector<int> vector1{1, 2, 3, 4, 5};
    std::vector<int> vector2{-1, 1, 2, -1, 3, 4, 5, -1};
    assert(VectorUtils::removeOccurrences(vector1, -1) == vector1);
    assert(VectorUtils::removeOccurrences(vector2, -1) == vector1);
}
