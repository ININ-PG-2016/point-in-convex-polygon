#pragma once

#include "PointInConvexPolygonTest.h"
#include <vector>

class O1PolarSubdivisionTest : public PointInConvexPolygonTest
{
private:
	Point2D centroid;
	int wedgesPerOctant;
	int getWedgeIndex(const Point2D & point);

	std::vector<Function>* wedges;

public:
	O1PolarSubdivisionTest(const Polygon & p) : PointInConvexPolygonTest(p), wedgesPerOctant(1) {}
	void setWedgesPerOctant(int wedges) { wedgesPerOctant = wedges; }
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
};