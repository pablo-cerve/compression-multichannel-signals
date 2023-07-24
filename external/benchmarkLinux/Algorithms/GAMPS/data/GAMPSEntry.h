#ifndef __GAMPSENTRY_H
#define __GAMPSENTRY_H

struct GAMPSEntry
{
public:
	double value;
	int endingTimestamp;

	GAMPSEntry(double v, int e)
	{
		value = v;
		endingTimestamp = e;
	};

	GAMPSEntry(){};
};

# endif