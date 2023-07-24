
#ifndef CPP_PROJECT_GROUP_GAMPS_H
#define CPP_PROJECT_GROUP_GAMPS_H


#include "coder_gamps.h"

#include "DataStream.h"
#include "GAMPS.h"

class GroupGAMPS {

private:
    CSVReader* input_csv;
    Mask* nodata_rows_mask;
    MappingTable* mapping_table;
    Dataset* dataset;
    double epsilon;
    int total_data_types;
    int data_type_index;
    int total_data_type_columns;

    CoderGAMPS* coder_gamps;
    GAMPSInput* gamps_input;
    GAMPS* gamps;

    void getNodataRowsMask(int column_index);
    GAMPSInput* getGAMPSInput();
    CDataStream* getColumn(int column_index);

public:
    GroupGAMPS(CoderGAMPS* coder_gamps_, double epsilon_);
    ~GroupGAMPS();
    GAMPSOutput* getGAMPSOutput(int column_index);
    Mask* getMask();

    static int mapValue(std::string csv_value, int dataset_offset);
    static std::string unmapValue(std::string csv_value, int dataset_offset);
};


#endif //CPP_PROJECT_GROUP_GAMPS_H
