
#ifndef CPP_PROJECT_MAPPING_TABLE_H
#define CPP_PROJECT_MAPPING_TABLE_H

#include "GAMPSOutput.h"
#include "map_entry.h"

class MappingTable {
private:
    std::vector<int> getRatioColumns(std::vector<int> base_column_index_vector, int column_index);


public:
    std::vector<int> base_columns_indexes;
    std::vector<int> nodata_columns_indexes;
    std::vector<MapEntry*> mapping_vector;
    int data_columns_count; // number of data columns (some of them can be nodata)
    int gamps_columns_count; // number of data columns which contain at least an integer value, used in the GAMPS algorithm

    // Only called in CoderGAMPS
    MappingTable();
    void setNoDataColumnsIndexes(std::vector<bool> nodata_columns, int data_columns_count_);
    void calculate(GAMPSOutput* gamps_output);
    int getColumnIndex(int gamps_col_index);
    int getRatioGampsColumnIndex(int column_index);

    // Only called in DecoderGAMPS
    MappingTable(std::vector<int> vector);


    void createBaseColumnIndex();

    bool isNodataColumnIndex(int col_index);

    std::vector<int> ratioColumns(int base_column_index);
    std::vector<int> baseColumnIndexVector();
    bool isBaseColumn(int column_index);
    void print(int total_groups, int group_index);

    static void printVector(std::vector<int> columns_indexes, int total_groups, int group_index);
    static int mapIndex(int table_index, int total_groups, int group_index);
};

#endif //CPP_PROJECT_MAPPING_TABLE_H
