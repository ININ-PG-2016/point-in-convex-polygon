#pragma once
#include "PointInConvexPolygonTest.h"

class HalfPlaneIntersectionTest :
	public PointInConvexPolygonTest
{
public:
	HalfPlaneIntersectionTest(const Polygon & p) : PointInConvexPolygonTest(p) {}
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
};

