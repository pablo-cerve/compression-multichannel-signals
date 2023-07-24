#ifndef __GAMPSOUTPUT_CPP
#define __GAMPSOUTPUT_CPP

#include "string.h"
#include "GAMPSOutput.h"
#include "../../../DataManagementLayer/Data/MultiDataStream.h"

GAMPSOutput::GAMPSOutput(GAMPSInput* gampsInput)
{
	m_pInputData = gampsInput;
}

GAMPSOutput::~GAMPSOutput()
{
	int baseCount = 0;
	int ratioCount = 0;

	for(int i = 0; i< TgoodSize;i++)
	{
		if(Tgood[i] == i)
			delete listResultBaseSignal[baseCount++];
		else
			delete listResultRatioSignal[ratioCount++];
	}

	if(baseCount > 0)	delete[] listResultBaseSignal;
	if(ratioCount > 0)	delete[] listResultRatioSignal;

	delete[] Tgood;
	delete m_pApproxData;
}

int* GAMPSOutput::getTgood()
{
	return this->Tgood;
}

void GAMPSOutput::setTgoodSize(int tGoodSize)
{
	TgoodSize = tGoodSize;
}

void GAMPSOutput::setTgood(int* temp)
{
	this->Tgood = temp;
}

CMultiDataStream* GAMPSOutput::getApproxData()
{
	return m_pApproxData;
}

// Get base signal result of GAMPS algorithm
DynArray<GAMPSEntry>** GAMPSOutput::getResultBaseSignal()
{
	return this->listResultBaseSignal;
}

// Get ratio signal result of GAMPS algorithm
DynArray<GAMPSEntry>** GAMPSOutput::getResultRatioSignal()
{
	return this->listResultRatioSignal;
}

DynArray<double>* GAMPSOutput::getResultBaseSignalEpsilon()
{
	return this->listResultBaseSignalEpsilon;
}

// Get ratio signal result of GAMPS algorithm
DynArray<double>*GAMPSOutput::getResultRatioSignalEpsilon()
{
	return this->listResultRatioSignalEpsilon;
}

// Set base signal result
void GAMPSOutput::setResultBaseSignal(DynArray<GAMPSEntry>** resultBaseSignal)
{
	this->listResultBaseSignal = resultBaseSignal;
}

// Set ratio signal result
void GAMPSOutput::setResultRatioSignal(DynArray<GAMPSEntry> **_resultRatioSignal)
{
	this->listResultRatioSignal = _resultRatioSignal;
}

void GAMPSOutput::setResultBaseSignalEpsilon(DynArray<double>* resultBaseSignalEpsilon)
{
	this->listResultBaseSignalEpsilon = resultBaseSignalEpsilon;
}

void GAMPSOutput::setResultRatioSignalEpsilon(DynArray<double>* resultRatioSignalEpsilon)
{
	this->listResultRatioSignalEpsilon = resultRatioSignalEpsilon;
}

void GAMPSOutput::setInputFileNames(char **_fileNames)
{
	inputFileNames = _fileNames;
}

//Decompress approximate data and recover the real data
void GAMPSOutput::decompressData()
{
	int numOfStream = TgoodSize;
	m_pApproxData = new CMultiDataStream(numOfStream);

	// Calculate approx of base data based on result base signals
	int baseCount = 0;
	int ratioCount = 0;
	int* decompressedSignal = new int[numOfStream];
	int numOfdecompressedSignal = 0;

	for(int i = 0; i < numOfStream; i++)
	{
		decompressedSignal[i] = 0;
		if(Tgood[i] == i)// Base signals
		{
			decompressedSignal[i] = 1;
			numOfdecompressedSignal ++;
			CDataStream* dataStream = new CDataStream();
			int currentPos = 0;
			DynArray<GAMPSEntry>* temp =  listResultBaseSignal[baseCount++];
			int tempCount = temp->size();
			int lastTimeStamp = temp->getAt(tempCount - 1 ).endingTimestamp;

			for(int j = 1; j <= lastTimeStamp; j++)
			{
				if(currentPos != tempCount - 1 && j > temp->getAt(currentPos).endingTimestamp)
					currentPos++;

				DataItem inputEntry;
				inputEntry.timestamp = j;
				inputEntry.value = temp->getAt(currentPos).value;
				dataStream->add(inputEntry);
			}

			m_pApproxData->setDataAt(i,dataStream);
		}
	}

	// From approx base data and result ratio signals, decompress for remaining signals
	while (numOfdecompressedSignal != numOfStream ){
		for(int i = 0; i < numOfStream; i++)
		{
			if(Tgood[i] != i && decompressedSignal[Tgood[i]] == 1)// Ratio signals
			{
				decompressedSignal[i] = 1;
				numOfdecompressedSignal ++;
				CDataStream* dataStream = new CDataStream();
				int baseSignalPos = Tgood[i];
				CDataStream* baseSignalStream = m_pApproxData->getDataAt(baseSignalPos);
				int currentPos = 0;
				DynArray<GAMPSEntry>* ratioTemp =  listResultRatioSignal[ratioCount++];
				int ratioTempCount = ratioTemp->size();
				int lastTimeStamp = ratioTemp->getAt(ratioTempCount - 1).endingTimestamp;

				for(int j = 1; j <= lastTimeStamp; j++)
				{
					if(currentPos != ratioTempCount - 1 && j > ratioTemp->getAt(currentPos).endingTimestamp)
						currentPos++;

					double baseSignalValue = baseSignalStream->getAt(j - 1).value;

					// in case of baseSignalValue ==0
					if (baseSignalValue < 1 && baseSignalValue > -1 )
						baseSignalValue =1;

					DataItem inputEntry;
					inputEntry.timestamp = j;
					inputEntry.value = ratioTemp->getAt(currentPos).value * baseSignalValue;
					dataStream->add(inputEntry);
				}

				m_pApproxData->setDataAt(i,dataStream);
			}
		}
	}

	delete [] decompressedSignal;
}

double GAMPSOutput::getCompressionRatio()
{
	// Calculate total memory input
	double returnValue = 0;
	int numOfStream = m_pInputData->getNumOfStream();
	int signalLength = m_pInputData->getOriginalStreams()->getDataAt(0)->size();
	int inputElementSize= DOUBLE_SIZE;
	int inputMemory = numOfStream * signalLength * inputElementSize; // all signal

	// Calculate total memory output
	int baseSignalOutputMemory = 0;
	int ratioSignalOutputMemory = 0;
	int baseSignalCount = 0;
	int ratioSignalCount = 0;
	int outputMemory = 0;
	int gampsEntrySize = DOUBLE_SIZE + INT_SIZE;

	// Do not care the Tgood size
	for(int i = 0 ; i < numOfStream; i++)
	{
		if(Tgood[i] == i)
		{
			baseSignalOutputMemory = baseSignalOutputMemory + listResultBaseSignal[baseSignalCount]->size() * gampsEntrySize;
			baseSignalCount++;
		}
		else
		{
			ratioSignalOutputMemory = ratioSignalOutputMemory + listResultRatioSignal[ratioSignalCount]->size() * gampsEntrySize;
			ratioSignalCount++;
		}
	}
	outputMemory = baseSignalOutputMemory + ratioSignalOutputMemory;

	returnValue = (double)outputMemory / (double)inputMemory;
	return returnValue;
}

double GAMPSOutput::getRMSE()
{
	// Calculate approx data
	decompressData();

	int numOfStream = TgoodSize;
	double returnValue = 0;
	for(int j = 0; j < numOfStream; j++)
	{
		double unitReturnValue = 0;

		// get jth approx stream
		CDataStream* decompressData = m_pApproxData->getDataAt(j);

		// get jth original stream
		CDataStream* inputData = m_pInputData->getOriginalStreams()->getDataAt(j);

		int dataSize = decompressData->size();
		for(int i = 0; i < dataSize; i++)
		{
			// get original data at ith timestamp of jth stream
			double inputValue = inputData->getAt(i).value;

			// get appox data at ith timestamp of jth stream
			double decompressValue = decompressData->getAt(i).value;

			double temp = decompressValue - inputValue;
			temp = pow(temp,2);
			unitReturnValue = unitReturnValue + temp;
		}

		unitReturnValue = sqrt(unitReturnValue / dataSize);
		unitReturnValue = unitReturnValue /(inputData->getMax() - inputData->getMin());
		returnValue = returnValue + unitReturnValue;
	}

	returnValue = returnValue / numOfStream;
	return returnValue;
}

// Purpose	: Get file path to save for approximate data and output
// Parameter: (In) : parameters: An array pointer which helds the file path
//			         paraCount : Number of output file paths
void GAMPSOutput::getParameter(char** parameters,int paraCount)
{
	if(paraCount == 3)
	{
		baseSignalPath = parameters[0];
		ratioSignalPath = parameters[1];
		appropriateDataPath = parameters[2];

	}
	else
	{
		printf("-----------------------Invalid parameters----------------------------\n");
		printf("GAMPSOutput must have 3 para: basePath,signalPath,appropriatePath \n");
		printf("---------------------------------------------------------------------\n");
	}
}

// Purpose	: Write output data and approximate data to file
void GAMPSOutput::write_output_to_File()
{
	write_baseSignal_to_File(baseSignalPath);
	write_RatioSignal_to_File(ratioSignalPath);
	write_ApproxData_to_File(appropriateDataPath);
}

// Purpose	: Write base signal output data after decompressing to file
// Parameter: prefixFilePath : the directory path and prefix name of all base signal output files
void GAMPSOutput::write_baseSignal_to_File(char* prefixFilePath)
{
	int count = 0;
	int numOfStream = m_pInputData->getNumOfStream();

	for(int j = 0; j < numOfStream; j++)
	{
		if(Tgood[j] == j)
		{
			DynArray<GAMPSEntry>* listBaseSignal = listResultBaseSignal[count++];
			char* fileName = new char[200];
			::memset(fileName,0,sizeof(char)*200);
			char* fileOrder = new char[10];
			::memset(fileOrder,0,sizeof(char)*10);

			strcat(fileName,prefixFilePath);
			sprintf(fileOrder, "%d", j + 1); // itoa (j + 1,fileOrder,10);
			strcat(fileName,fileOrder);
			strcat(fileName,".txt");
			FILE *file = fopen(fileName, "w");
			if (file == NULL) perror ("Error opening file");
			else
			{
				char* buffer = new char[30];
				::memset(buffer,0,sizeof(char)*30);
				char* date = new char[10];
				::memset(date,0,sizeof(char)*10);
				char* num = new char[20];
				::memset(num,0,sizeof(char)*20);
				for (int i = 0; i < listBaseSignal->size(); i++)
				{
					strcpy(buffer, "");
					GAMPSEntry entry = listBaseSignal->getAt(i);
					sprintf(date, "%d", entry.endingTimestamp); // itoa(entry.endingTimestamp, date, 10);
					sprintf(num,"%.5f", entry.value);
					strcat(buffer, date);
					strcat(buffer,",");
					strcat(buffer, num);

					fputs(buffer, file);
					if (i != listBaseSignal->size() -1)
					{
						fputs("\n", file);
					}
				}

				delete[] buffer;
				delete[] date;
				delete[] num;
				fclose (file);
			}
			delete[] fileName;
			delete[] fileOrder;
		}
	}
}

// Purpose	: Write ratio signal output data after decompressing to file
// Parameter: prefixFilePath : the directory path and prefix name of all ratio signal output files
void GAMPSOutput::write_RatioSignal_to_File(char* prefixFilePath)
{
	int count = 0;
	int numOfStream = m_pInputData->getNumOfStream();

	for(int i = 0; i< numOfStream; i++)
	{
		int baseSignalOrderNum = 0;
		int ratioSignalOrderNum = 0;

		if(Tgood[i] != i)
		{
			ratioSignalOrderNum = i;
			baseSignalOrderNum = Tgood[i];

			char* fileName = new char[200];
			::memset(fileName,0,sizeof(char)*200);
			char* ratioOrder = new char[10];
			::memset(ratioOrder,0,sizeof(char)*10);
			char* baseOrder = new char[10];
			::memset(baseOrder,0,sizeof(char)*10);

			DynArray<GAMPSEntry>* listRatioSignal = listResultRatioSignal[count++];

			strcat(fileName,prefixFilePath);
			sprintf(ratioOrder, "%d", ratioSignalOrderNum + 1); // itoa (ratioSignalOrderNum + 1,ratioOrder,10);
			strcat(fileName,ratioOrder);
			strcat(fileName,"_");
			sprintf(baseOrder, "%d", baseSignalOrderNum + 1); // itoa (baseSignalOrderNum + 1,baseOrder,10);
			strcat(fileName,baseOrder);
			strcat(fileName,".txt");
			FILE *file = fopen(fileName, "w");
			if (file == NULL) perror ("Error opening file");
			else
			{
				char *buffer = new char[30];
				::memset(buffer,0,sizeof(char)*30);
				char *date = new char[10];
				::memset(date,0,sizeof(char)*10);
				char *num = new char[20];
				::memset(num,0,sizeof(char)*20);

				for (int j = 0; j < listRatioSignal->size(); j++)
				{
					strcpy(buffer, "");
					GAMPSEntry entry = listRatioSignal->getAt(j);
					sprintf(date, "%d", entry.endingTimestamp); // itoa(entry.endingTimestamp, date, 10);
					sprintf(num,"%.5f", entry.value);
					strcat(buffer, date);
					strcat(buffer,",");
					strcat(buffer, num);

					fputs(buffer, file);
					if (j != listRatioSignal->size() -1)
					{
						fputs("\n", file);
					}
				}
				delete[] buffer;
				delete[] date;
				delete[] num;
				fclose (file);
			}

			delete[] fileName;
			delete[] baseOrder;
			delete[] ratioOrder;
		}
	}
}

// Purpose	: Write approx data after decompressing to file
// Parameter: prefixPath : the directory path and prefix name of all approx data output files
void GAMPSOutput::write_ApproxData_to_File(char* prefixPath)
{
	for(int j = 0 ; j < m_pApproxData->getNumOfStream() ; j++)
	{
		char* fileName = new char[200];
		::memset(fileName,0,sizeof(char)*200);

		CDataStream* dataStream = m_pApproxData->getDataAt(j);
		strcpy(fileName, "");
		strcat(fileName,prefixPath);
		strcat(fileName,inputFileNames[j]);
		FILE *file = fopen(fileName, "w");
		if (file == NULL) perror ("Error opening file");
		else
		{
			char *buffer = new char[30];
			::memset(buffer,0,sizeof(char)*30);
			char *date = new char[10];
			::memset(date,0,sizeof(char)*10);
			char *num = new char[20];
			::memset(num,0,sizeof(char)*20);

			for (int i = 0; i< dataStream->size(); i++)
			{
				strcpy(buffer, "");
				DataItem entry = dataStream->getAt(i);
				sprintf(date, "%d", entry.timestamp); // itoa(entry.timestamp, date, 10);
				sprintf(num,"%.5f", entry.value);
				strcat(buffer, num);

				fputs(buffer, file);
				if (i != dataStream->size() -1)
				{
					fputs("\n", file);
				}
			}
			delete[] buffer;
			delete[] date;
			delete[] num;
			fclose (file);
		}
		delete[] fileName;
	}
}

#endif
