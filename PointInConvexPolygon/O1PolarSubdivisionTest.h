#pragma once

#include "PointInConvexPolygonTest.h"
#include <vector>

class O1PolarSubdivisionTest : public PointInConvexPolygonTest
{
private:
	Point2D centroid;
	int wedgesPerOctant;
	int getWedgeIndex(const Point2D & point);
	int wedgesPerOctantTimes4Minus1;
	int wedgesPerOctantTimes2Minus1;
	int wedgesPerOctantTimes2;
	int wedgesPerOctantTimes8Minus1;

	std::vector<int>* wedges;
	Function* edgeFunctions;

public:
	O1PolarSubdivisionTest(const class Polygon & p) : PointInConvexPolygonTest(p) { setWedgesPerOctant(p.vertices.size() / 8); }
	void setWedgesPerOctant(int wedges);
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
};