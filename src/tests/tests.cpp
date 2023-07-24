
#include "tests.h"

#include "tests_datetime_utils.h"
#include "tests_string_utils.h"
#include "tests_math_utils.h"
#include "tests_vector_utils.h"
#include "tests_line.h"
#include "tests_utils.h"
#include "tests_bit_stream.h"
#include "tests_datetime_utils.h"
#include "tests_modelKT.h"
#include "tests_examples.h"

void Tests::runAll() {
    std::cout << "TestsDatetimeUtils::runAll();" << std::endl; TestsDatetimeUtils::runAll();
    std::cout << "TestsMathUtils::runAll();" << std::endl;     TestsMathUtils::runAll();
    std::cout << "TestsStringUtils::runAll();" << std::endl;   TestsStringUtils::runAll();
    std::cout << "TestsVectorUtils::runAll();" << std::endl;   TestsVectorUtils::runAll();

    std::cout << "TestsBitStream::runAll();" << std::endl;     TestsBitStream::runAll();
    std::cout << "TestsLine::runAll();" << std::endl;          TestsLine::runAll();
    std::cout << "TestsModelKT().runAll();" << std::endl;      TestsModelKT().runAll();

    std::cout << "TestsExamples::runAll();" << std::endl;      TestsExamples::runAll();

    std::cout << std::endl << "SUCCESSFUL TEST RUN!" << std::endl;
}
