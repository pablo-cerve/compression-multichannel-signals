
#include "group_gamps.h"
#include "coder_utils.h"

GroupGAMPS::GroupGAMPS(CoderGAMPS* coder_gamps_, double epsilon_){
    coder_gamps = coder_gamps_;
    epsilon = epsilon_;

    input_csv = coder_gamps->input_csv;
    mapping_table = coder_gamps->mapping_table;
    dataset = coder_gamps->dataset;
    total_data_types = coder_gamps->total_data_types;
    data_type_index = coder_gamps->data_type_index;
    total_data_type_columns = coder_gamps->total_data_type_columns;
}

GroupGAMPS::~GroupGAMPS(){
    // free memory as in benchmarkLinux
    delete gamps_input;
    // delete gamps;
}

GAMPSOutput* GroupGAMPS::getGAMPSOutput(int column_index){
    getNodataRowsMask(column_index);
    gamps_input = getGAMPSInput();
    gamps = new GAMPS(epsilon, gamps_input);
    gamps->compute();
    GAMPSOutput* gamps_output = gamps->getOutput();
    return gamps_output;
}

Mask* GroupGAMPS::getMask(){
    return nodata_rows_mask;
}

void GroupGAMPS::getNodataRowsMask(int column_index){
    nodata_rows_mask = new Mask();
    std::vector<bool> nodata_columns(total_data_type_columns, true);

    input_csv->goToFirstDataRow(column_index);
    while (input_csv->continue_reading) {
        std::vector<std::string> row = input_csv->readLineCSV();
        bool nodata_row = true;
        int j = 0;
        for(int i = data_type_index; i < row.size(); i+=total_data_types){ // i > 0 to skip TimeDelta column
            std::string csv_value = row.at(i);
            if (!Constants::isNoData(csv_value)){
                nodata_row = false;
                nodata_columns[j] = false;
            }
            j++;
        }
        nodata_rows_mask->add(nodata_row);
    }
    nodata_rows_mask->close();
    mapping_table->setNoDataColumnsIndexes(nodata_columns, total_data_type_columns);
}

GAMPSInput* GroupGAMPS::getGAMPSInput(){
    int data_columns_count = total_data_type_columns - mapping_table->nodata_columns_indexes.size();
    CMultiDataStream* multiStream = new CMultiDataStream(data_columns_count);
    int j = 1;
    for(int i = data_type_index; i <= dataset->data_columns_count; i+=total_data_types){
        if (mapping_table->isNodataColumnIndex(j++)) { continue; } // skip nodata columns
        dataset->setColumn(i);
        CDataStream* signal = getColumn(i);
        multiStream->addSingleStream(signal);
    }
    return new GAMPSInput(multiStream);
}

CDataStream* GroupGAMPS::getColumn(int column_index){
//    std::cout << "BEGIN getColumn" << std::endl;
    CDataStream* dataStream = new CDataStream();

    nodata_rows_mask->reset();
    int first_timestamp = 1;  // the first timestamp is always 1
    int timestamp = first_timestamp - 1;
    int previous_value = -1;
    int current_value;

    input_csv->goToFirstDataRow(column_index);
    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        if (nodata_rows_mask->isNoData()){ continue; } // skip nodata rows

        timestamp++;

        if (Constants::isNoData(csv_value)){
            if (previous_value == -1){ continue; } // up to this point no integer has been read
            current_value = previous_value; // same as the previous integer value
        }
        else {
            current_value = GroupGAMPS::mapValue(csv_value, coder_gamps->dataset->offset());
            if (previous_value == -1 && timestamp > first_timestamp){
                // the first rows of the column were nodata so we must fill them with current_value
                for(int i = first_timestamp; i < timestamp; i++){ dataStream->add(DataItem(current_value, i)); }
            }
            previous_value = current_value;
        }
        // std::cout << "add(DataItem(" << current_value << ", " << timestamp << ")" << std::endl;
        dataStream->add(DataItem(current_value, timestamp));
    }
    assert(timestamp > 0);
    assert(previous_value > -1);
    assert(timestamp == dataStream->size());
    assert(timestamp == coder_gamps->data_rows_count - nodata_rows_mask->total_no_data);
    return dataStream;
}

// static
int GroupGAMPS::mapValue(std::string csv_value, int dataset_offset){
    return CoderUtils::mapValueInt(csv_value, dataset_offset + 1);
}

std::string GroupGAMPS::unmapValue(std::string csv_value, int dataset_offset){
    return CoderUtils::unmapValue(csv_value, dataset_offset + 1);
}
