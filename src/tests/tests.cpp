
#include "tests.h"

#include "tests_datetime_utils.h"
#include "tests_string_utils.h"
#include "tests_math_utils.h"
#include "tests_vector_utils.h"
#include "tests_line.h"
#include "tests_bit_stream.h"
#include "tests_datetime_utils.h"
#include "tests_modelKT.h"
#include "tests_examples.h"

void Tests::runAll(std::string full_dataset_path) {
    bool some_error = false;
    bool with_full_dataset_path = full_dataset_path.size() > 0;

    std::cout << "TestsDatetimeUtils::runAll" << std::endl;
    try { TestsDatetimeUtils::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    std::cout << "TestsMathUtils::runAll" << std::endl;
    try { TestsMathUtils::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    std::cout << "TestsStringUtils::runAll" << std::endl;
    try { TestsStringUtils::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    std::cout << "TestsVectorUtils::runAll" << std::endl;
    try { TestsVectorUtils::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    std::cout << "TestsLine::runAll" << std::endl;
    try { TestsLine::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    std::cout << "TestsModelKT::runAll" << std::endl;
    try { TestsModelKT::runAll(); }
    catch(std::exception& e) { some_error = printError(e); }

    if (with_full_dataset_path){
        std::cout << "TestsBitStream::runAll" << std::endl;
        try { TestsBitStream::runAll(full_dataset_path); }
        catch(std::exception& e) { some_error = printError(e); }

        if (!some_error){
            // Do not run these tests if any of the previous tests fails
            // (in that case these tests will almost certainly also fail)
            std::cout << "TestsExamples::runAll" << std::endl;
            try { TestsExamples::runAll(full_dataset_path); }
            catch(std::exception& e) { some_error = printError(e); }
        }
    }

    std::string common_str = "******************************************************";
    std::string result_str = "***** Successful test run! All the tests passed. *****";
    if (some_error) {
        result_str = "***** Unsuccessful test run! Some tests failed. ******";
    }
    std::cout << common_str << std::endl;
    std::cout << result_str << std::endl;
    std::cout << common_str << std::endl;
    if (!with_full_dataset_path){
        std::cout << "IMPORTANT: The datasets path can be passed as the last argument in order to run additional tests." << std::endl;
    }
}

bool Tests::printError(std::exception e){
    std::cout << "ERROR: " + std::string(e.what()) << std::endl;
    return true;
}
