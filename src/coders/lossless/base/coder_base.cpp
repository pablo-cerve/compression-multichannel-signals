
#include "coder_base.h"

#include "assert.h"
#include "conversor.h"

void CoderBase::codeColumnBefore() {}

void CoderBase::codeColumnWhile(std::string csv_value){
    codeValueRaw(csv_value);
}

void CoderBase::codeColumnAfter() {}

std::vector<int> CoderBase::codeTimeDelta(CoderBase* coder){
    CSVReader* input_csv = coder->input_csv;

    std::vector<int> time_delta_vector{};
    input_csv->goToFirstDataRow(0);
    bool first_value = true;
    while (input_csv->continue_reading){
        std::string csv_value = input_csv->readNextValue();
        int csv_value_int = Conversor::stringToInt(csv_value);
        time_delta_vector.push_back(csv_value_int);

        if (first_value){
            // the first value is always 0, no need to encode anything
            assert(csv_value == "0");
            first_value = false;
            continue;
        }

        // same code as CoderBase::codeColumnWhile
        coder->codeValueRaw(csv_value);
    }
    return time_delta_vector;
}
