#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "../../DataStructures/DynArray/DynArray.cpp"

class Output
{
public:
	virtual double getCompressionRatio()=0;
	virtual double getUpdateFrequency()=0;
	virtual double getRMSE()=0;
	virtual void write_output_to_File()=0;
	virtual void getParameter(char** parameters,int paraCount)=0;
};

#endif
