
#ifndef CPP_PROJECT_TESTS_BIT_STREAM_H
#define CPP_PROJECT_TESTS_BIT_STREAM_H

#include "bit_stream_reader.h"
#include "bit_stream_writer.h"

class TestsBitStream {

public:
    static void runAll();

private:
    static const std::string TESTS_BIT_STREAM_PATH;
    static void floatTest();
    static void doubleTest();

};

#endif //CPP_PROJECT_TESTS_BIT_STREAM_H
