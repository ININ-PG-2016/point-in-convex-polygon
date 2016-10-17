#pragma once

#include "PointInConvexPolygonTest.h"

class CrossProductTest : public PointInConvexPolygonTest
{
private:
	Point2D* edgeVectors;

public:
	CrossProductTest(const Polygon & p) : PointInConvexPolygonTest(p) {}
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
};