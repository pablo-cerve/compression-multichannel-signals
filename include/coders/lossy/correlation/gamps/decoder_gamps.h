
#ifndef CPP_PROJECT_DECODER_GAMPS_H
#define CPP_PROJECT_DECODER_GAMPS_H

#include "decoder_common.h"
#include "mapping_table.h"
#include "GAMPSOutput.h"

class DecoderGAMPS: public DecoderCommon {

private:
    bool limit_mode = true;
    std::vector<std::vector<std::string>> columns;
    int column_index;

    int total_groups;
    int group_index;
    int total_group_columns; // total number of columns in a group (same for every group)

    MappingTable* mapping_table;

    void decodeDataRows(bool is_lossless_) override;

    void decodeTimeDeltaColumn();
    void decodeGroup();
    void decodeMappingTable();
    void decodeNoDataColumns();
    void decodeGAMPSColumns();
    std::vector<std::string> decodeBaseColumn(std::vector<double> & base_column_double);
    std::vector<std::string> decodeRatioColumn(std::vector<double> base_column_double);
    std::vector<double> decodeGAMPSRatioColumn();
    void decodeWindowRatioColumn(std::vector<double> & column);
    void decodeConstantWindowRatioColumn(std::vector<double> & column, int window_size);

public:
    using DecoderCommon::DecoderCommon;

};

#endif //CPP_PROJECT_DECODER_GAMPS_H
