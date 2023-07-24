
#ifndef MODEL_KT_DOT_H
#define MODEL_KT_DOT_H

#include <iostream>
#include <stdexcept>
#include "model_metrics.h"
#include "modelA.h"

template<typename CODE_VALUE_ = unsigned int,
        int CODE_VALUE_BITS_ = (std::numeric_limits<CODE_VALUE_>::digits + 3) / 2,
        int FREQUENCY_BITS_ = std::numeric_limits<CODE_VALUE_>::digits - CODE_VALUE_BITS_>
struct modelKT : public model_metrics<CODE_VALUE_, CODE_VALUE_BITS_, FREQUENCY_BITS_>
{
    typedef model_metrics<CODE_VALUE_, CODE_VALUE_BITS_, FREQUENCY_BITS_> metrics;
    typedef typename metrics::prob prob;
    typedef CODE_VALUE_ CODE_VALUE;
    using metrics::MAX_CODE;
    using metrics::MAX_FREQ;
    using metrics::CODE_VALUE_BITS;
    using metrics::ONE_FOURTH;
    using metrics::ONE_HALF;
    using metrics::THREE_FOURTHS;
    //
    // variables used by the model
    //
    modelA<int, 16, 14> model_data;
    modelA<int, 16, 14> model_no_data;
    bool no_data; // current state (false for data, true for no_data)

    modelKT()
    {
        reset();
        no_data = true;
    }
    void reset()
    {
        model_data.reset();
        model_no_data.reset();
    }
    prob getProbability(int c)
    {
        prob p = no_data ? model_no_data.getProbability(c) : model_data.getProbability(c);
        no_data = c;
        return p;
    }
    prob getChar(CODE_VALUE scaled_value, int &c)
    {
        prob p = no_data ? model_no_data.getChar(scaled_value, c) : model_data.getChar(scaled_value, c);
        no_data = c;
        return p;
    }
    CODE_VALUE getCount()
    {
        int count = no_data ? model_no_data.getCount() : model_data.getCount();
        return count;
    }

};

#endif //#ifndef MODEL_KT_DOT_H
