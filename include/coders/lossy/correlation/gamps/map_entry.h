
#ifndef CPP_PROJECT_MAP_ENTRY_H
#define CPP_PROJECT_MAP_ENTRY_H

#include <iostream>
#include <vector>

class MapEntry {
public:
    // index of the column in the dataset file
    // column_index >= 1 since the delta time column is ignored
    int column_index;

    // if base_column_index == 0 then the MapEntry corresponds to a nodata column
    // else if base_column_index == column_index then the MapEntry corresponds to a base column
    // else the MapEntry corresponds to a ratio column and base_column_index is the index of its base column
    int base_column_index;

    // if base_column_index == column_index then this vector has the indexes of all its ratio columns
    // else this vector is empty
    std::vector<int> ratio_columns;

    MapEntry(int column_index_, int base_column_index_, std::vector<int> ratio_columns_);
    bool isNodataEntry();
    bool isBaseColumn();
    bool isRatioColumn();
    void print(int total_groups, int group_index);
};

#endif //CPP_PROJECT_MAP_ENTRY_H
