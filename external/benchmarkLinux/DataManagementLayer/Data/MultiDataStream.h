#ifndef __MULTISTREAM_H
#define __MULTISTREAM_H

#include "DataStream.h"

class CMultiDataStream
{
private:
	CDataStream** data;
	int numOfStream;
	int count;

public:
	CMultiDataStream(int numOfStream); 
	~CMultiDataStream(void);

	CDataStream** getData();
	CDataStream* getDataAt(int position);
	void setDataAt(int position, CDataStream* stream);
	void addSingleStream(CDataStream* singleStream);// Add stream to the end of current data

	int getNumOfStream();
	int getDataLength();

	void addOutlier(int percent);// Modify some data values to be the outliers by a random percent

	void write_to_File(char* path);
};

#endif 