
#ifndef CPP_PROJECT_DECODER_COMMON_H
#define CPP_PROJECT_DECODER_COMMON_H

#include "bit_stream_reader.h"
#include "csv_writer.h"
#include "dataset.h"
#include "constants.h"
#include "mask.h"

class DecoderCommon {

private:
    virtual void decodeDataRows(bool mask_mode_) = 0;
    std::string decodeValue(int y);
    int decodeRaw();
    void closeFiles();

protected:
    CSVWriter* output_csv;
    Dataset* dataset;
    int window_size;
    std::string coder_name;

    void transposeMatrix(int data_rows_count_, std::vector<std::vector<std::string>> columns, int total_columns);

public:
    BitStreamReader* input_file;
    int data_rows_count;
    std::vector<int> time_delta_vector;
    int window_size_bit_length;
    int row_index;

    // The following two attributes are only used in mask mode
    Mask* mask;
    std::vector<Mask*> masks_vector;

    //
    // main methods
    //
    static std::string decodeCoderName(BitStreamReader* input_file);
    bool isDecoder(std::string coder_name_);
    DecoderCommon(std::string coder_name_, BitStreamReader* input_file_, CSVWriter* output_csv_);
    void decodeWindowParameter();
    void decode();

    //
    // auxiliary methods
    //
    std::string getDatasetName();
    void setWindowSizeBitLength(int window_size_bit_length_); // used by TimeDeltaDecoder::decode
    bool decodeBool();
    int decodeInt(int bits);
    int decodeWindowLength(int window_size_bit_length);
    int decodeWindowLength();
    int decodeUnary();
    int decodeUnaryInv();
    std::string decodeValueRaw();
    float decodeFloat();
    void flushByte();
};

#endif //CPP_PROJECT_DECODER_COMMON_H
