
#include "mapping_table.h"
#include "vector_utils.h"

MappingTable::MappingTable(){}

void MappingTable::setNoDataColumnsIndexes(std::vector<bool> nodata_columns, int data_columns_count_){
    for (int i = 0; i < nodata_columns.size(); i++){
        int col_index = i + 1;
        if (nodata_columns[i]) { // column with index col_index is a nodata column
            nodata_columns_indexes.push_back(col_index);
        }
    }
    data_columns_count = data_columns_count_;
    gamps_columns_count = data_columns_count_ - nodata_columns_indexes.size();
#if CHECKS
    assert(data_columns_count > 0);
    assert(gamps_columns_count > 0);
#endif
}

void MappingTable::calculate(GAMPSOutput* gamps_output){
    int data_column_index = 0;
    for(int i=1; i <= data_columns_count; i++){
        MapEntry* map_entry;
        std::vector<int> ratio_signals;

        if (VectorUtils::vectorIncludesInt(nodata_columns_indexes, i)){
            map_entry = new MapEntry(i, 0, ratio_signals);
        }
        else {
            int base_index = gamps_output->getTgood()[data_column_index];
            int base_index_mapped = getColumnIndex(base_index);

            // add ratio signals
            for(int j=0; j < gamps_columns_count; j++){
                int base_j = gamps_output->getTgood()[j];
                if (base_j != j && base_j == data_column_index){
                    int j_index = getColumnIndex(j);
                    ratio_signals.push_back(j_index);
                }
            }
            map_entry = new MapEntry(i, base_index_mapped, ratio_signals);
            data_column_index++;
        }
        mapping_vector.push_back(map_entry);
    }
    createBaseColumnIndex();
}

MappingTable::MappingTable(std::vector<int> vector){
    for(int i = 0; i < vector.size(); i++){
        MapEntry* map_entry;
        std::vector<int> ratio_signals;

        int col_index = i + 1;
        int base_index = vector.at(i);

        if (base_index == 0){ // nodata column
            nodata_columns_indexes.push_back(col_index);
        }
        else { // data column
            // add ratio signals
            for (int j = 0; j < vector.size(); j++) {
                int col_index_j = j + 1;
                int base_index_j = vector.at(j);
                if (col_index_j != col_index && base_index_j == col_index){
                    ratio_signals.push_back(col_index_j);
                }
            }
        }
        map_entry = new MapEntry(col_index, base_index, ratio_signals);
        mapping_vector.push_back(map_entry);
    }
    createBaseColumnIndex();
    data_columns_count = vector.size();
    gamps_columns_count = data_columns_count - nodata_columns_indexes.size();
#if CHECKS
    assert(data_columns_count > 0);
    assert(gamps_columns_count > 0);
#endif
}

void MappingTable::createBaseColumnIndex(){
    for (int i = 0; i < mapping_vector.size(); i++){
        int col_index = i + 1;
        if (VectorUtils::vectorIncludesInt(base_columns_indexes, col_index)) { continue; }
        for (int j = 0; j < mapping_vector.size(); j++){
            MapEntry* map_entry = mapping_vector.at(j);
            if (map_entry->base_column_index == col_index && !VectorUtils::vectorIncludesInt(base_columns_indexes, col_index)){
                base_columns_indexes.push_back(col_index);
                continue;
            }
        }
    }
}

std::vector<int> MappingTable::getRatioColumns(std::vector<int> base_column_index_vector, int column_index){
    std::vector<int> ratio_signals;
    for (int j = 0; j < base_column_index_vector.size(); j++){
        int column_index_j = j + 1;
        int base_column_index_j = base_column_index_vector.at(j);
        if (base_column_index_j == column_index && base_column_index_j != column_index_j){
            ratio_signals.push_back(column_index_j);
        }
    }
    return ratio_signals;
}

bool MappingTable::isNodataColumnIndex(int col_index){
    return VectorUtils::vectorIncludesInt(nodata_columns_indexes, col_index);
}

int MappingTable::getColumnIndex(int gamps_col_index){
    int data_column_index = 0;

    int i = 1;
    while (i <= data_columns_count) {
        if (!VectorUtils::vectorIncludesInt(nodata_columns_indexes, i)){
            if (data_column_index == gamps_col_index) { break; }
            data_column_index++;
        }
        i++;
    }
    return i;
}

int MappingTable::getRatioGampsColumnIndex(int column_index){
    int ratio_index = 0;
    for (int i = 0; i < mapping_vector.size(); i++){
        MapEntry* map_entry = mapping_vector.at(i);
        if (map_entry->isRatioColumn()){
            if (map_entry->column_index == column_index) { break; }
            ratio_index++;
        }
    }
    return ratio_index;
}

std::vector<int> MappingTable::ratioColumns(int base_column_index){
    return mapping_vector.at(base_column_index - 1)->ratio_columns;
}

std::vector<int> MappingTable::baseColumnIndexVector(){
    std::vector<int> res;
    for (int i = 0; i < mapping_vector.size(); i++){
        MapEntry* map_entry = mapping_vector.at(i);
        res.push_back(map_entry->base_column_index);
    }
    return res;
}

bool MappingTable::isBaseColumn(int column_index){
    return mapping_vector.at(column_index - 1)->base_column_index == column_index;
}

void MappingTable::print(int total_groups, int group_index){
    std::cout << "------------------------" << std::endl;
    std::cout << "MappingTable" << std::endl;
    std::cout << "nodata_columns_indexes = "; printVector(nodata_columns_indexes, total_groups, group_index);
    std::cout << "base_columns_indexes = "; printVector(base_columns_indexes, total_groups, group_index);
    for(int i=0; i < mapping_vector.size(); i++){
        mapping_vector.at(i)->print(total_groups, group_index);
    }
    std::cout << "------------------------" << std::endl;
}

void MappingTable::printVector(std::vector<int> columns_indexes, int total_groups, int group_index){
    std::vector<int> mapped_vector;
    for(int i=0; i < columns_indexes.size(); i++){
        int mapped_value = mapIndex(columns_indexes.at(i), total_groups, group_index);
        mapped_vector.push_back(mapped_value);
    }
    VectorUtils::printIntVector(mapped_vector);
}

int MappingTable::mapIndex(int table_index, int total_groups, int group_index){
    return (table_index - 1) * total_groups + group_index;
}
