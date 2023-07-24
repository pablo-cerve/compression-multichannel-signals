#ifndef __LINEARBUCKET_H
#define __LINEARBUCKET_H

#pragma once
#include "../../../DataStructures/DynArray/DynArray.cpp"
#include "../../../DataStructures/Polygon/CPolygon.h"
#include "../../../DataStructures/Line/Line.h"

class LinearBucket
{
private:
	CPolygon* polygon;
	double esp;

	double getDistance(Point* p, Line* l);

	// Calculate max distance from any point in hull to 2 points
	double getMaxDisForEdge(Point* vertex1, Point* vertex2);

public:
	LinearBucket();
	LinearBucket(double e);
	~LinearBucket(void);

	int getSize();

	// Get approximated line of this bucket
	void getAproximatedLine(Point& vertex1, Point& vertex2);
	void getAproximatedLineMOD(Point& vertex1, Point& vertex2, int x_coord); /////////////////////////////////////// MOD

	void addPoint(double value);
	void addPointMOD(double time, double value); /////////////////////////////////////////////////////////////////// MOD
	void removePoint();

	void resetBucket();

	// Check if bucket is valid with respect to error tolerance
	bool checkEpsConstraint();
};

#endif
