
#include "tests_bit_stream.h"
#include "bit_stream_utils.h"
#include <cfloat>
#include "assert.h"
#include <iostream>
#include "tests_utils.h"

const std::string TestsBitStream::TESTS_BIT_STREAM_PATH = TestsUtils::OUTPUT_PATH + "/tests_bit_stream";

void TestsBitStream::runAll(){
    std::cout << "  floatTest();" << std::endl;  floatTest();
    std::cout << "  doubleTest();" << std::endl; doubleTest();
}

void TestsBitStream::floatTest(){
    Path coded_path = Path(TESTS_BIT_STREAM_PATH, "testFloat.code");
    BitStreamWriter* bit_stream_writer = new BitStreamWriter(coded_path);
    float a = 0.238728932739; bit_stream_writer->pushFloat(a);
    float b = 0.2893232; bit_stream_writer->pushFloat(b);
    float c = 203020323.22; bit_stream_writer->pushFloat(c);
    float d = FLT_MAX; bit_stream_writer->pushFloat(d);
    delete bit_stream_writer;

    BitStreamReader* bit_stream_reader = new BitStreamReader(coded_path);
    float diff = 0.00000000000000000000000000000001;
    float a_deco = bit_stream_reader->getFloat(); assert(a_deco - a < diff);
    float b_deco = bit_stream_reader->getFloat(); assert(b_deco - b < diff);
    float c_deco = bit_stream_reader->getFloat(); assert(c_deco - c < diff);
    float d_deco = bit_stream_reader->getFloat(); assert(d_deco == d);
    delete bit_stream_reader;
    BitStreamUtils::removeFile(coded_path);
}

void TestsBitStream::doubleTest(){
    Path coded_path = Path(TESTS_BIT_STREAM_PATH, "testDouble.code");
    BitStreamWriter* bit_stream_writer = new BitStreamWriter(coded_path);
    double a = 0.238728932739; bit_stream_writer->pushDouble(a);
    double b = 0.2893232; bit_stream_writer->pushDouble(b);
    double c = 203020323.22; bit_stream_writer->pushDouble(c);
    double d = DBL_MAX; bit_stream_writer->pushDouble(d);
    delete bit_stream_writer;

    BitStreamReader* bit_stream_reader = new BitStreamReader(coded_path);
    float diff = 0.00000000000000000000000000000001;
    double a_deco = bit_stream_reader->getDouble(); assert(a_deco - a < diff);
    double b_deco = bit_stream_reader->getDouble(); assert(b_deco - b < diff);
    double c_deco = bit_stream_reader->getDouble(); assert(c_deco - c < diff);
    double d_deco = bit_stream_reader->getDouble(); assert(d_deco == d);
    delete bit_stream_reader;
    BitStreamUtils::removeFile(coded_path);
}
