#ifndef __POLYGON_CPP
#define __POLYGON_CPP

#include "../../stdafx.h"
#include "CPolygon.h"

// Add ----> A
//#include <crtdbg.h>
// Add <---- A

//Add ----> A
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Add <---- A

CPolygon::CPolygon(void)
{
	left = NULL;
	right = NULL;
}

CPolygon::~CPolygon(void)
{
	for(int i = 0; i < allPoints.size(); i++)
	{
		delete allPoints.getAt(i);
	}
}

int CPolygon::getSize()
{
	return allPoints.size();
}

DynArray<Point*>* CPolygon::getConvexHull()
{
	return &convexHull;
}

// Add new point to current polygon
void CPolygon::addPoint(double time, double value)
{
	Point* p = new Point(value, time);
	allPoints.add(p);
	partitionPoints();
	updateConvexHull();
}

// Remove last point (right-most) from allPoints
void CPolygon::removePoint()
{
	delete allPoints.getAt(allPoints.size() - 1);
	allPoints.remove();
	partitionPoints();
	updateConvexHull();
}

// Check the direction of one point to 2 two points
// Parameter:
//            Point* p0: given first point
//            Point* p1: given second point
//            Point* p2: checked point
// Return   :
//            double value: >=0    Clockwise
//                          <0     Opposite clockwise
double CPolygon::checkClockwise(Point* p0, Point* p1, Point* p2)
{
	return (p0->x - p1->x) * (p2->y - p1->y) - (p2->x - p1->x) * (p0->y - p1->y);
}

// Partition all points into 2 section upperPoint and lowerPoint
void CPolygon::partitionPoints()
{
	int size = allPoints.size();
	if (size == 1)	left = allPoints.getAt(0);
	if (size > 1)	right = allPoints.getAt(size - 1);

	upperPoints.clear();
	lowerPoints.clear();
	Point* item;

	// Partition all points
	for (int i = 1 ; i < (size - 1) ; i++ )
	{
		item = allPoints.getAt(i);
		double dir = checkClockwise(left, right, item);
		if (dir < 0)
			upperPoints.add(item);
		else
			lowerPoints.add(item);
	}
}

// Update upperHull or lowerHull
// Parameter:
//            DynArray<Point*>* input: points contain hull
//            int factor: -1    output is upperHull
//                         1    output is lowerHull
void CPolygon::updateHalfConvexHull(DynArray<Point*>* input, int factor)
{
	DynArray<Point*>* output;
	if (factor == - 1)
		output = &upperHull;
	else
		output = &lowerHull;

	Point* item;
	if (left != NULL)	output->add(left);
	if (right != NULL)	input->add(right);

	int inputSize = input->size();
	for (int i = 0; i < inputSize; i++)
	{
		item = input->getAt(i);

		// the last point always belongs to convex hull
		// since it is right-most point
		output->add(item);

		// remove intermediate points until we have convex hull
		while(output->size() >= 3)
		{
			int end = output->size() - 1;

			// Check if the 'end - 1' point creates concave hull then remove it
			if (factor * checkClockwise(output->getAt(end - 2), output->getAt(end), output->getAt(end - 1)) <= 0)
			{
				output->replace(end - 1, output->getAt(end));
				output->remove();
			}
			else
				break;
		}
	}
}

// Update all convex hull
void CPolygon::updateConvexHull()
{
	convexHull.clear();
	upperHull.clear();
	lowerHull.clear();
	updateHalfConvexHull(&upperPoints, -1);
	updateHalfConvexHull(&lowerPoints, 1);

	Point* item;

	int end = upperHull.size();
	for(int i = 0; i < end; i++)
	{
		item = upperHull.getAt(i);
		convexHull.add(item);
	}

	end = lowerHull.size() - 2;
	for(int i = end; i > 0; i --)
	{
		item = lowerHull.getAt(i);
		convexHull.add(item);
	}
}

#endif
