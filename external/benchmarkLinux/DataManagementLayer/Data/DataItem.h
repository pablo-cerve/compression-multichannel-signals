#ifndef __DATAITEM_H
#define __DATAITEM_H

struct DataItem
{
	double value;
	int timestamp;

	DataItem(){};

	DataItem(double v, int t)
	{
		value = v;
		timestamp = t;
	}
};

#endif