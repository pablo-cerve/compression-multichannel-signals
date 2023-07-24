
#include <coders/correlation/gamps/mapping_table.h>
#include "map_entry.h"
#include "vector_utils.h"

MapEntry::MapEntry(int column_index_, int base_column_index_, std::vector<int> ratio_columns_){
    column_index = column_index_;
    base_column_index = base_column_index_;
    ratio_columns = ratio_columns_;
}

bool MapEntry::isNodataEntry(){
    return base_column_index == 0;
}

bool MapEntry::isBaseColumn(){
    return column_index == base_column_index;
}

bool MapEntry::isRatioColumn(){
    return !(isNodataEntry() || isBaseColumn());
}

void MapEntry::print(int total_groups, int group_index){
    std::cout << "column_index = " << MappingTable::mapIndex(column_index, total_groups, group_index);
    if (isNodataEntry()) {
        std::cout << " [nodata column]" << std::endl;
        return;
    }
    std::cout << ", base_column_index = " << MappingTable::mapIndex(base_column_index, total_groups, group_index);
    if (isBaseColumn()){
        std::cout << ", ratio_columns = "; MappingTable::printVector(ratio_columns, total_groups, group_index);
    }
    else{
        std::cout << std::endl;
    }
}
