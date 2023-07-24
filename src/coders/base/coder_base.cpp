
#include "coder_base.h"

void CoderBase::codeColumnBefore() {}

void CoderBase::codeColumnWhile(std::string csv_value){
    codeValueRaw(csv_value);
}

void CoderBase::codeColumnAfter() {}
