#ifndef __GAMPSOUTPUT_H
#define __GAMPSOUTPUT_H

#include <stdio.h>
#include "../../../stdafx.h"
#include "../../../DataStructures/DynArray/DynArray.cpp"
#include "GAMPSEntry.h"
#include "../../../DataManagementLayer/Data/Output.h"
#include "GAMPSInput.h"

class GAMPSOutput: public Output
{
private:
	GAMPSInput* m_pInputData;

	// Compress data is the combination of base and ratio signals
	DynArray<GAMPSEntry>** listResultBaseSignal;
	DynArray<GAMPSEntry>** listResultRatioSignal;

	// Arrays with the respective epsilons for the base and ratio signals
	DynArray<double>* listResultBaseSignalEpsilon;
	DynArray<double>* listResultRatioSignalEpsilon;

	// Contains correspondence between base and ratio signals
	int TgoodSize;
	int* Tgood;

	CMultiDataStream* m_pApproxData;

	// location for result files
	char* baseSignalPath;
	char* ratioSignalPath;
	char* appropriateDataPath;
	char** inputFileNames;

public:
	GAMPSOutput(GAMPSInput* data);
	~GAMPSOutput();

	int* getTgood();
	void setTgood(int* temp);
	void setTgoodSize(int tGoodSize);

	CMultiDataStream* getApproxData();
	DynArray<GAMPSEntry>** getResultBaseSignal();
	DynArray<GAMPSEntry>** getResultRatioSignal();

	DynArray<double>* getResultBaseSignalEpsilon();
	DynArray<double>* getResultRatioSignalEpsilon();

	void setResultBaseSignal(DynArray<GAMPSEntry>** resultBaseSignal);
	void setResultRatioSignal(DynArray<GAMPSEntry>** resultRatioSignal);
	void setInputFileNames(char** fileNames);

	void setResultBaseSignalEpsilon(DynArray<double>* resultBaseSignalEpsilon);
	void setResultRatioSignalEpsilon(DynArray<double>* resultRatioSignalEpsilon);

	//Decompress approximate data and recover the real data
	void decompressData();

	virtual double getCompressionRatio();
	virtual double getUpdateFrequency(){return 0;};
	virtual double getRMSE();

	//Get file path to save for approximate data and output
	virtual void getParameter(char** parameters,int paraCount);

	// Write results to files
	virtual void write_output_to_File();
	void write_baseSignal_to_File(char* filePath);
	void write_RatioSignal_to_File(char* filePath);
	void write_ApproxData_to_File(char* path);

};

#endif
