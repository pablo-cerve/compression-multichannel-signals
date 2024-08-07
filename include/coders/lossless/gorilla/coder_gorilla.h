
#ifndef CPP_PROJECT_CODER_GORILLA_H
#define CPP_PROJECT_CODER_GORILLA_H

#include "coder_cols.h"
#include "gorilla_constants.h"

class CoderGorilla: public CoderCols {

public:
    using CoderCols::CoderCols;
    static std::vector<int> codeTimeDelta(CoderCols* coder);

private:
    static constexpr uint32_t kMaxLeadingZerosLength = (1 << GorillaConstants::kLeadingZerosLengthBits) - 1;

    uint64_t previousValue_;
    uint8_t previousValueLeadingZeros_;
    uint8_t previousValueTrailingZeros_;

    bool read_first_value;
    int no_data_value;

    void codeColumnBefore() override;
    void codeColumnWhile(std::string csv_value) override;
    void codeColumnAfter() override;
    static void codeD(CoderCols* coder, int current_delta, int previous_delta);
};

#endif //CPP_PROJECT_CODER_GORILLA_H
