#ifndef __SLIDEFILTERSENTRY_H
#define __SLIDEFILTERSENTRY_H

#include "../../../DataStructures/Line/Line.h"

struct SlideFiltersEntry
{
	bool connToFollow; // boolean flag indicating this item is connected to next item
	double value;
	double timestamp;

	SlideFiltersEntry()
	{
		timestamp = 0;
		value = 0;
		connToFollow = false;
	}

	SlideFiltersEntry(double v, double t, bool conn)
	{
		connToFollow = conn;
		value = v;
		timestamp = t;
	}

	SlideFiltersEntry(Point& point, bool conn)
	{
		connToFollow = conn;
		value = point.y;
		timestamp = point.x;
	}

	SlideFiltersEntry(SlideFiltersEntry& original)
	{
		this->connToFollow = original.connToFollow;
		this->value = original.value;
		this->timestamp = original.timestamp;
	}

};

#endif
