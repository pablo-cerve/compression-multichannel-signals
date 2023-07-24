#ifndef __DATASTREAM_H
#define __DATASTREAM_H

#include "DataItem.h"
#include "../../DataStructures/DynArray/DynArray.cpp"

class CDataStream: public DynArray<DataItem>
{
private:
	double min;
	double max;
	double mean;
	double variance;

public:
	double getMin(); // Get minimum value of data stream
	double getMax(); // Get maximum value of data stream
	double getMean(); // Get mean value of data stream
	double getVariance(); // Get variance of data stream

	double* getDataHistogram(int numOfBin); // Get data histogram by counting the value in each 'bin' interval
	void statistic(); // Calculate min, max, mean, variance value

	void addOutlier(int percent); // Modify some data values to be the outliers by a random percent

	void resize(int newSize); // Resize the current data stream to new size
//	void write_to_File(char* path); // Write data stream to file
};

#endif
