#ifndef __SLIDEFILTERSOUPUT_CPP
#define __SLIDEFILTERSOUPUT_CPP

#include "../../../stdafx.h"
#include "../Data/SlideFiltersOutPut.h"

// Add ----> A
//#include <crtdbg.h>
// Add <---- A

//Add ----> A
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Add <---- A

SlideFiltersOutput::SlideFiltersOutput(SlideFiltersInput *originalData)
{
	m_pInputData = originalData;
	m_pCompressData = new DynArray<SlideFiltersEntry>();
}

SlideFiltersOutput::~SlideFiltersOutput()
{
	delete m_pCompressData;
	delete m_pApproxData;
}

DynArray<SlideFiltersEntry>* SlideFiltersOutput::getCompressData()
{
	return m_pCompressData;
}

// Calculate approximation data from model parameters
void SlideFiltersOutput::decompressData()
{
	m_pApproxData = new CDataStream();
	int size = m_pCompressData->size();
	int position = 0;
	double timeStamp = 0;
	int lastTimeStamp = m_pCompressData->getAt(m_pCompressData->size() - 1).timestamp;
	SlideFiltersEntry slEntry1, slEntry2;
	Line* l = NULL;
	DataItem inputEntry;

	for(int i = 0; i < lastTimeStamp; i++)
	{
		//Read compressed data
		if (i >= timeStamp)
		{
			slEntry1 = m_pCompressData->getAt(position);

			if (slEntry1.connToFollow)//Connected
			{
				position++;
				slEntry2 = m_pCompressData->getAt(position);

				//Go back for second reading
				if (slEntry2.connToFollow)
				{
					timeStamp = slEntry2.timestamp - 1;
				}
				else
				{
					position++;
					timeStamp = slEntry2.timestamp;
				}
			}
			else //Disconnected
			{
				inputEntry.timestamp = slEntry1.timestamp;
				inputEntry.value = slEntry1.value;
				m_pApproxData->add(inputEntry);
				break;
			}

			//Create line go through two point in compressed data
			if (l != NULL)
			{
				delete l;
			}

			Point p1(slEntry1.value, slEntry1.timestamp);
			Point p2(slEntry2.value, slEntry2.timestamp);
			l = new Line(&p1, &p2);
		}

		//Get point on line at each corresponding time
		inputEntry.timestamp = i + 1;
		inputEntry.value = l->getValue(inputEntry.timestamp);
		m_pApproxData->add(inputEntry);
	}

	delete l;
}

double SlideFiltersOutput::getCompressionRatio()
{
	// Calculate original data size
	double returnValue = 0;
	int signalLength = m_pInputData->getDataLength();
	int inputElementSize= DOUBLE_SIZE;
	int inputMemory = signalLength * inputElementSize;

	// Calculate compress data size
	int compressLength = m_pCompressData->size();
	int outputElementSize = (2 * DOUBLE_SIZE + 1);
	int outputMemory = compressLength * outputElementSize;

	returnValue =(double)outputMemory / (double)inputMemory;
	return returnValue;
}

double SlideFiltersOutput::getUpdateFrequency()
{
	double returnValue = 0;
	int signalLength = m_pInputData->getDataLength();

	// Get number of segments
	int segNum = 0;
	for (int i=0; i< m_pCompressData->size(); i++)
	{
		SlideFiltersEntry* sfEntry = &(m_pCompressData->getAt(i));
		//if (sfEntry.connToFollow == true){
        if (sfEntry->connToFollow == true){
			segNum ++;
		}
	}
	returnValue =(double)segNum / (double)signalLength;
	return returnValue;
}

double SlideFiltersOutput::getRMSE()
{
	// Decompress data
	decompressData();

	// Calculate RMSE
	double returnValue = 0;
	int dataSize = m_pInputData->getDataLength();


	for(int i = 0; i < dataSize; i++)
	{
		double inputValue = m_pInputData->getAt(i).value;
		double decompressValue = m_pApproxData->getAt(i).value;
		double temp = decompressValue - inputValue;
		temp = pow(temp,2);
		returnValue = returnValue + temp;
	}
	returnValue = sqrt(returnValue / dataSize);

	//Normalize RMSE with respect to data range
	return returnValue/(m_pInputData->getMaxValue() - m_pInputData->getMinValue());
}

// Purpose	: Get file path to save for approximate data and output
// Parameter: (In) : parameters: An array pointer which helds the file path
//			         paraCount : Number of output file paths
void SlideFiltersOutput::getParameter(char **parameters, int paraCount)
{
	if(paraCount == 2)
	{
		outputFilePath = parameters[0];
		appropriateFilePath = parameters[1];
	}
	else
	{
		printf("-----------------------Invalid parameters----------------------------\n");
		printf("SlideFilters must have 2 para: outputFilePath,appropriatePath \n");
		printf("---------------------------------------------------------------------\n");
	}
}

// Write compressed data and approximate data
void SlideFiltersOutput::write_output_to_File()
{
	write_output_to_File(outputFilePath);
	write_ApproxData_to_File(appropriateFilePath);
}

// Write compressed data
void SlideFiltersOutput::write_output_to_File(char* filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL) perror ("Error opening file");
	else
	{
		for (int i=0; i < m_pCompressData->size(); i++)
		{
			char *buffer = new char[50];
			::memset(buffer,0,sizeof(char)*50);
			char *date = new char[20];
			::memset(date,0,sizeof(char)*20);
			char *num = new char[20];
			::memset(num,0,sizeof(char)*20);
			char *conn = new char[20];
			::memset(conn,0,sizeof(char)*20);
			//SlideFiltersEntry entry = m_pCompressData->getAt(i);
            SlideFiltersEntry* entry = &(m_pCompressData->getAt(i));
			sprintf(date,"%.9f", entry->timestamp);
			sprintf(num,"%.9f", entry->value);
			if(entry->connToFollow)
			{
				strcpy(conn,"True");
			}
			else
			{
				strcpy(conn,"False");
			}
			strcpy(buffer, date);
			strcat(buffer,",");
			strcat(buffer, num);
			strcat(buffer,",");
			strcat(buffer, conn);

			fputs(buffer, file);
			if (i != m_pCompressData->size() -1)
			{
				fputs("\n", file);
			}

			delete buffer;
			delete date;
			delete num;
			delete conn;
		}
		fclose (file);
	}
}

// Write approximate data
void SlideFiltersOutput::write_ApproxData_to_File(char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL) perror ("Error opening file");
	else
	{
		for (int i=0; i < m_pApproxData->size(); i++)
		{
			char *buffer = new char[50];
			::memset(buffer,0,sizeof(char)*50);
			char *date = new char[20];
			::memset(date,0,sizeof(char)*20);
			char *num = new char[20];
			::memset(num,0,sizeof(char)*20);
			DataItem entry = m_pApproxData->getAt(i);
			itoa(entry.timestamp, date, 10);
			sprintf(num,"%.9f", entry.value);
			strcat(buffer, num);

			fputs(buffer, file);

			if (i != m_pApproxData->size() -1)
			{
				fputs("\n", file);
			}

			delete num;
			delete buffer;
			delete date;
		}
		fclose (file);
	}
}

#endif
