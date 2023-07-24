#ifndef __SLIDEFILTERSOUPUT_H
#define __SLIDEFILTERSOUPUT_H

#include "../../../stdafx.h"
#include "stdlib.h"
#include "../../../DataManagementLayer/Data/Output.h"
#include "SlideFiltersEntry.h"
#include "../../../DataManagementLayer/Data/DataStream.h"
#include "SlideFiltersInput.h"

class SlideFiltersOutput : public Output
{
private:
	SlideFiltersInput* m_pInputData;

	DynArray<SlideFiltersEntry>* m_pCompressData;
	CDataStream* m_pApproxData;

	// location for result files
	char* outputFilePath;
	char* appropriateFilePath;

public:
	SlideFiltersOutput(SlideFiltersInput* originalData);
	~SlideFiltersOutput(void);

	DynArray<SlideFiltersEntry>* getCompressData();

	// Calculate approximation data from model parameters
	void decompressData();

	virtual double getCompressionRatio();
	virtual double getUpdateFrequency();
	virtual double getRMSE();

	// Get file path to save for approximate data and output
	virtual void getParameter(char** parameters,int paraCount);

	// Write compressed data and approximate data
	virtual void write_output_to_File();

	// Write compressed data
	void write_output_to_File(char* filename);

	// Write approximate data
	void write_ApproxData_to_File(char* filename);
};

#endif
