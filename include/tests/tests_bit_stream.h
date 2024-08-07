
#ifndef CPP_PROJECT_TESTS_BIT_STREAM_H
#define CPP_PROJECT_TESTS_BIT_STREAM_H

#include "bit_stream_reader.h"
#include "bit_stream_writer.h"

class TestsBitStream {

public:
    static void runAll(std::string full_dataset_path);

private:
    static void floatTest(std::string full_dataset_path);
    static void doubleTest(std::string full_dataset_path);

};

#endif //CPP_PROJECT_TESTS_BIT_STREAM_H
