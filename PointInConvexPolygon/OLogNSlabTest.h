#pragma once

#include "PointInConvexPolygonTest.h"
#include "Geometry.h"

class OLogNSlabTest : public PointInConvexPolygonTest
{
private:
	int maxYIndex;
	int minYIndex;

	struct Slab
	{
		double yTop;
		Function f1;
		Function f2;
	};

	Slab* slabs;

public:
	OLogNSlabTest(const Polygon & p) : PointInConvexPolygonTest(p) {}
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
};