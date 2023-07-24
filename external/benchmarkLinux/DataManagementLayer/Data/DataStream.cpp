#ifndef __DataStream_CPP
#define __DataStream_CPP

#include "../../stdafx.h"
#include "math.h"
#include "DataStream.h"

double CDataStream::getMin()
{
	return min;
}

double CDataStream::getMax()
{
	return max;
}

double CDataStream::getMean()
{
	return mean;
}

double CDataStream::getVariance()
{
	return variance;
}

// Get data histogram by counting the value in each 'bin' interval
double* CDataStream::getDataHistogram(int numOfBin)
{
	// Initialize intervals to be zero
	int* intervals =  new int[numOfBin];
	double* result = new double[numOfBin];
	int dataSize = this->size();
	for(int i = 0; i < numOfBin; i++)
	{
		intervals[i] = 0;
	}

	// Calculate length of interval
	double distance = (max - min)/numOfBin;

	// Count values for each interval
	for(int i = 0; i < dataSize; i++)
	{
		DataItem entry = this->getAt(i);
		double curValue = entry.value;
		if(curValue == max)
		{
			intervals[numOfBin- 1] = intervals[numOfBin- 1] + 1;
		}
		else
		{
			int k = (curValue - min)/distance;
			intervals[k] = intervals[k] + 1;
		}
	}

	// Calculate histogram values in terms of percentage
	for(int i = 0; i < numOfBin; i++)
	{
		result[i] = (double)intervals[i] /(double)dataSize;
	}

	delete[] intervals;
	return result;
}

// Calculate min, max, mean, variance value
void CDataStream::statistic()
{
//    printf("Call statistic\n");
	// Find minimum and maximum value and sum
	int dataSize = this->size();
	double total = 0;

	if(dataSize > 0)
	{
		DataItem entry = this->getAt(0);
		double temp = entry.value;
		min = max = temp;
	}

	for(int i = 1; i < dataSize; i++)
	{
		DataItem entry = this->getAt(i);
		double temp = entry.value;
        //printf("%f\n",temp);
		total = total +  temp;
		if(temp > max && temp <20000) max = temp;
		if(temp < min) min = temp;
	}

	// Calculate mean
	mean = total / dataSize;

	// Calculate variance
	variance = 0;
	for(int i = 0; i < dataSize; i++)
	{
		DataItem entry = this->getAt(i);
		double temp = entry.value;
		double var = temp - mean;
		variance = variance + pow(var,2);
	}
	variance = variance / dataSize;
	variance = sqrt(variance);
}

// Modify some data values to be the outliers by a random percent
// The value of outlier is 20000
void CDataStream::addOutlier(int percent)
{
	int dataSize = this->size();
	for(int i = 0; i < dataSize; i++)
	{
		DataItem entry = this->getAt(i);
		DataItem newEntry;
		newEntry.timestamp = entry.timestamp;
		int r = rand() % 100;
		if (r >= 50 && r < (50 + percent) ){
			newEntry.value = 20000;
			this->updateAt(i,newEntry);
		}
	}
}

void CDataStream::resize(int newSize)
{
	length = newSize;
	statistic();
}

//void CDataStream::write_to_File(char* path)
//{
//	FILE *file = fopen(path, "w");
//	if (file == NULL) perror ("Error opening file");
//	else
//	{
//		char *buffer = new char[30];
//		::memset(buffer,0,sizeof(char)*30);
//		char *date = new char[10];
//		::memset(date,0,sizeof(char)*10);
//		char *num = new char[20];
//		::memset(num,0,sizeof(char)*20);
//
//		for (int i = 0; i < this->size(); i++)
//		{
//			strcpy(buffer,"");
//			DataItem entry = this->getAt(i);
//			itoa(entry.timestamp, date, 10);
//			sprintf(num,"%.5f", entry.value);
//			strcat(buffer, date);
//			strcat(buffer,",");
//			strcat(buffer, num);
//
//			fputs(buffer, file);
//			if (i != this->size() -1)
//			{
//				fputs("\n", file);
//			}
//		}
//
//		delete[] buffer;
//		delete[] date;
//		delete[] num;
//		fclose (file);
//	}
//}

#endif
