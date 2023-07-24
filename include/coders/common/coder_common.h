
#ifndef CPP_PROJECT_CODER_COMMON_H
#define CPP_PROJECT_CODER_COMMON_H

#include <string>
#include "bit_stream_writer.h"
#include "csv_reader.h"
#include "dataset.h"
#include "constants.h"
#include "window.h"

class CoderCommon {

private:
    virtual void codeDataRows(bool base_coder_) = 0;
    //
    // This method maps a value read in the csv file into an integer to be written in the output file.
    //
    int codeValue(std::string x);
    void codeRaw(int value);
    void closeFiles();

protected:
    int window_size;
    std::string coder_name;

public:
    CSVReader* input_csv;
    BitStreamWriter* output_file;
    Dataset* dataset;
    int data_rows_count;

    //
    // main methods
    //
    CoderCommon(std::string coder_name_, CSVReader* input_csv_, BitStreamWriter* output_file_);
    void codeCoderName();
    void codeWindowParameter();
    Dataset* code();

    //
    // auxiliary methods
    //
    void codeBit(int bit);
    void codeBits(int bit, int times);
    void codeBool(bool bit);
    void codeInt(int value, int bits);
    void codeWindowLength(Window* window);
    void codeUnary(int value);
    void codeValueRaw(std::string x);
    void codeFloat(float x);
    void flushByte();
};

#endif //CPP_PROJECT_CODER_COMMON_H
