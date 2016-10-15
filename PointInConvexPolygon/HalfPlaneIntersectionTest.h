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

private:
	void prepareImplicitLine(const Point2D &start, const Point2D &end, Function *function);
	int vertexCount;
	Function *functions;
};

