#ifndef __Line_H
#define __Line_H

#include "../../DataManagementLayer/Data/DataItem.h"

// Add ----> debug
//#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Add <---- debug


struct Point
{
	double x;
	double y;

	Point(){};

	Point(double v, double t)
	{
		x = t;
		y = v;
	}

	Point(DataItem& input)
	{
		x = input.timestamp;
		y = input.value;
	}
};


class Line
{
private:
	double slope;
	double intercept;

public:
	Line();
	Line(Point* point1, Point* point2);
	Line(Point* point1, double slope);
	Line(double slope, double intercept);
	Line(Line* original);

	void update(Point* point1, Point* point2);
	void update(Point* point1, double slope);

	double getSlope();
	void setSlope(double slope);

	double getIntercept();
	void setIntercept(double intercept);

	double getValue(double x);

	Point getIntersection(const Line& line);

    // New Methods
	double getYProjection(Point* point);
    double getYDistanceToDot(Point* point);
    bool pointIsAbove(Point* point);
    bool pointIsBelow(Point* point);

};

#endif
