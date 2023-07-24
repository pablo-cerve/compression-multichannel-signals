#ifndef __SLIDEFILTERSINPUT_H
#define __SLIDEFILTERSINPUT_H

#pragma once

#include "../../../DataManagementLayer/Data/Input_SingleStreamAlg.h"

class SlideFiltersInput : public Input_SingleStreamAlg
{
/*protected:
    CDataStream *originalStream;*/
public:
	SlideFiltersInput(CDataStream* _rawData, double _esp): Input_SingleStreamAlg(_rawData, _esp){}
    //SlideFiltersInput(CDataStream* data);
/*    ~SlideFiltersInput();
    CDataStream *getOriginalStream();
    double getMaxValue();
    double getMinValue();*/
};

#endif
