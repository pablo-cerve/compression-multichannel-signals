#ifndef __MULTISTREAM_CPP
#define __MULTISTREAM_CPP

#include "../../stdafx.h"
#include "MultiDataStream.h"
#include <cstring>

CMultiDataStream::CMultiDataStream(int numOfStream)
{
	this->numOfStream = numOfStream;
	data = new CDataStream*[numOfStream];
	count = 0;
}

CMultiDataStream::~CMultiDataStream(void)
{
	for(int i = 0; i < numOfStream; i++)
	{
		delete data[i];
	}

	delete[] data;
}

CDataStream** CMultiDataStream::getData()
{
	return data;
}

CDataStream* CMultiDataStream::getDataAt(int position)
{
	assert (position >= 0 && position < numOfStream);
	return data[position];
}

void CMultiDataStream::setDataAt(int position, CDataStream* stream)
{
	assert (position >= 0 && position < numOfStream);
	data[position] = stream;
}

// Add stream to the end of current data
void CMultiDataStream::addSingleStream(CDataStream* singleStream)
{
	data[count++] = singleStream;
	singleStream->statistic();

	// Normalize all streams' size to be equal
	if (count > 1 && (data[count - 2]->size() != singleStream->size()))
	{
		int minLength = data[count - 2]->size();
		if (singleStream->size() < minLength)
			minLength =  singleStream->size();
		for (int i = 0; i < count; i++)
		{
			data[i]->reduceSize(minLength);
		}
	}
}

int CMultiDataStream::getNumOfStream()
{
	return this->numOfStream;
}

int CMultiDataStream::getDataLength()
{
	if(count > 0) return data[0]->size();//All streams' sizes are equal, only need to get first one
	return 0;
}

// Modify some data values to be the outliers by a random percent
void CMultiDataStream::addOutlier(int percent)
{
	for(int j = 0; j < numOfStream; j++)
	{
		CDataStream* dataStream = this->data[j];
		dataStream->addOutlier(percent);
	}
}

void CMultiDataStream::write_to_File(char* path)
{
	for(int j = 0; j < numOfStream; j++)
	{
		CDataStream* dataStream = this->data[j];
		char* fileName = new char();
		strcat(fileName,path);
		char* bufferFile = new char();

		sprintf(bufferFile, "%d", j); // itoa (j,bufferFile,10);
		strcat(fileName,bufferFile);
		strcat(fileName,".txt");

		FILE *file = fopen(fileName, "w");
		if (file == NULL) perror ("Error opening file");
		else
		{
			int size = dataStream->size();
			for (int i=0; i< size; i++)
			{
				char *buffer = new char();
				char *date = new char();
				char *num = new char();
				DataItem entry = dataStream->getAt(i);
				sprintf(date, "%d", entry.timestamp); // itoa(entry.timestamp, date, 10);
				sprintf(num,"%.5f", entry.value);
				strcat(buffer, date);
				strcat(buffer,",");
				strcat(buffer, num);

				fputs(buffer, file);
				if (i != size -1)
				{
					fputs("\n", file);
				}
			}
			fclose (file);
		}
	}
}

#endif
