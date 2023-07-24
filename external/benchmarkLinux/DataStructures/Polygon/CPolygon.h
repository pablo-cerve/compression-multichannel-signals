#ifndef __POLYGON_H
#define __POLYGON_H

#pragma once
#include "../Line/Line.h"
#include "../DynArray/DynArray.cpp"

class CPolygon
{
private:
	DynArray<Point*> allPoints; // order by X. allPoints[0] is left-most point & allPoints[length -1] is right-most point
	DynArray<Point*> upperPoints; // order by X
	DynArray<Point*> lowerPoints; // order by X

	Point* left;
	Point* right;

	DynArray<Point*> convexHull;
	DynArray<Point*> upperHull;
	DynArray<Point*> lowerHull;

	// Check the direction of one point to 2 two points
	double checkClockwise(Point* p0, Point* p1, Point* p2);

	// Partition all points into 2 section upperPoint and lowerPoint
	void partitionPoints();

	// Update upperHull or lowerHull
	void updateHalfConvexHull(DynArray<Point*>* input, int factor);

	// Update all convex hull
	void updateConvexHull();

public:
	CPolygon(void);
	~CPolygon(void);

	int getSize();
	DynArray<Point*>* getConvexHull();

	// Add new point to current polygon
	void addPoint(double time, double value);

	// Remove last point from allPoints
	void removePoint();
};

#endif
