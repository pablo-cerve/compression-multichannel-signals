
#include "coder_gamps.h"
#include "time_delta_coder.h"
#include "arithmetic_mask_coder.h"
#include "conversor.h"
#include "assert.h"
#include "vector_utils.h"
#include "coder_apca.h"
#include "GAMPSInput.h"
#include "group_gamps.h"

void CoderGAMPS::setCoderParams(int window_size_, std::vector<int> epsilons_vector_){
    window_size = window_size_;
    epsilons_vector = epsilons_vector_;
}

void CoderGAMPS::codeDataRows(bool mask_mode_){
    codeTimeDeltaColumn();

    ArithmeticMaskCoder* amc = new ArithmeticMaskCoder(this, dataset->data_columns_count);
    total_data_rows_vector = amc->code();

    total_data_types = limit_mode ? dataset->dataColumnsGroupCount() : 1;
    total_data_type_columns = dataset->data_columns_count / total_data_types;

    gamps_epsilons_vector = getGAMPSEpsilonsVector();
    assert(gamps_epsilons_vector.size() == total_data_types);

    for(data_type_index = 1; data_type_index <= total_data_types; data_type_index++){
        codeDataTypeColumns();
    }
}

std::vector<int> CoderGAMPS::getGAMPSEpsilonsVector(){
    std::vector<int> new_epsilons_vector(total_data_types, -1);
    for(int i=1; i < epsilons_vector.size(); i++){ // skip the first entry (time delta epsilon)
        int data_type_index = (i - 1) % total_data_types; // -1 because the first entry is skipped
        int current_epsilon = new_epsilons_vector.at(data_type_index);
        int candidate_epsilon = epsilons_vector.at(i);
        if (current_epsilon == -1 || candidate_epsilon < current_epsilon){
            new_epsilons_vector.at(data_type_index) = candidate_epsilon;
        }
    }
    return new_epsilons_vector;
}

void CoderGAMPS::codeTimeDeltaColumn(){
    column_index = 0;
    dataset->setColumn(column_index);
    dataset->setMode("DATA");
    TimeDeltaCoder::code(this);
}

void CoderGAMPS::codeDataTypeColumns(){
    mapping_table = new MappingTable();
    double epsilon = (double) gamps_epsilons_vector.at(data_type_index - 1);
    GroupGAMPS* group_gamps = new GroupGAMPS(this, epsilon);
    GAMPSOutput* gamps_output = group_gamps->getGAMPSOutput(column_index);
    nodata_rows_mask = group_gamps->getMask();
    codeMappingTable(gamps_output);
    codeGAMPSColumns(gamps_output);
    delete group_gamps;
}

void CoderGAMPS::codeMappingTable(GAMPSOutput* gamps_output){
    mapping_table->calculate(gamps_output);
    // mapping_table->print(total_groups, group_index);
    std::vector<int> vector = mapping_table->baseColumnIndexVector();
    int vector_size = vector.size();
    int column_index_bit_length = MathUtils::bitLength(vector_size);
    for (int i = 0; i < vector_size; i++){
        codeInt(vector.at(i), column_index_bit_length);
    }
}

void CoderGAMPS::codeGAMPSColumns(GAMPSOutput* gamps_output){
    DynArray<GAMPSEntry>** base_signals = gamps_output->getResultBaseSignal();
    DynArray<GAMPSEntry>** ratio_signals = gamps_output->getResultRatioSignal();
    DynArray<double>* base_signals_epsilons = gamps_output->getResultBaseSignalEpsilon();
    DynArray<double>* ratio_signals_epsilons = gamps_output->getResultRatioSignalEpsilon();

    DynArray<GAMPSEntry>* column;
    double base_epsilon, ratio_epsilon;

    int base_index = 0;
    for(int i = 0; i < mapping_table->gamps_columns_count; i++){
        int table_index = mapping_table->getColumnIndex(i);
        if (!mapping_table->isBaseColumn(table_index)){ continue; }

        column_index = MappingTable::mapIndex(table_index, total_data_types, data_type_index);
        dataset->setColumn(column_index);
        column = base_signals[base_index];
        base_epsilon = base_signals_epsilons->getAt(base_index);
        base_index++;

        codeGAMPSColumn(column, base_epsilon); // CODE BASE COLUMN

        std::vector<int> ratio_columns = mapping_table->ratioColumns(table_index);
        for (int j = 0; j < ratio_columns.size(); j++){
            table_index = ratio_columns.at(j);
            column_index = MappingTable::mapIndex(table_index, total_data_types, data_type_index);
            dataset->setColumn(column_index);
            int ratio_index = mapping_table->getRatioGampsColumnIndex(table_index);
            column = ratio_signals[ratio_index];
            ratio_epsilon = ratio_signals_epsilons->getAt(ratio_index);

            codeGAMPSColumn(column, ratio_epsilon); // CODE RATIO COLUMN
        }
    }
}

void CoderGAMPS::codeGAMPSColumn(DynArray<GAMPSEntry>* column, double epsilon){
    total_data_rows_vector.at(column_index - 1);
    bool mask_mode = true;

    dataset->setMode("DATA");

    int entry_index = 0;
    GAMPSEntry current_entry = column->getAt(entry_index);
    epsilon = 0;
    int remaining = current_entry.endingTimestamp;
    APCAWindow* window = new APCAWindow(window_size, epsilon, mask_mode);
    nodata_rows_mask->reset();
    row_index = 0;
    input_csv->goToFirstDataRow(column_index);
    while (input_csv->continue_reading) {
        std::string csv_value = input_csv->readNextValue();
        row_index++;

        bool no_data_row = nodata_rows_mask->isNoData(); // all the values in the row for the column group are "N"
        bool no_data = Constants::isNoData(csv_value); // the value in the row is "N"

        // skip no_data
        if (no_data_row) { continue; }
        else if (no_data) {
            update(column, entry_index, current_entry, remaining);
            continue;
        }
        // convert double to string
        if (!no_data) { csv_value = Conversor::doubleToString(current_entry.value); }

        if (!window->conditionHolds(csv_value)) {
            codeWindow(window);
            window->addFirstValue(csv_value);
        }
        if (!no_data_row){
            update(column, entry_index, current_entry, remaining);
        }
    }
    if (!window->isEmpty()) {
        codeWindow(window);
    }
}

void CoderGAMPS::update(DynArray<GAMPSEntry>* column, int & entry_index, GAMPSEntry & current_entry, int & remaining){
    remaining--;
    if (remaining > 0){ return; }

    entry_index++;
    if (entry_index == column->size()) { return; }

    int previous_last_timestamp = current_entry.endingTimestamp;
    current_entry = column->getAt(entry_index);
    remaining = current_entry.endingTimestamp - previous_last_timestamp;
}

void CoderGAMPS::codeWindow(APCAWindow* window){
    codeWindowLength((Window*) window);
    std::string constant_value = window->constant_value;

    double value = Constants::NO_DATA_DOUBLE;
    if (!Constants::isNoData(constant_value)){
        value = Conversor::stringToDouble(constant_value);
    }
    codeFloat((float) value);
}
