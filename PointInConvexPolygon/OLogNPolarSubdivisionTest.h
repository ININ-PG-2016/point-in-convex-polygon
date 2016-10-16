#pragma once
#include "PointInConvexPolygonTest.h"

class OLogNPolarSubdivisionTest :
	public PointInConvexPolygonTest
{
public:
	OLogNPolarSubdivisionTest(const Polygon & p) : PointInConvexPolygonTest(p) {}
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
	
private:
	void prepareImplicitLine(const Point2D &start, const Point2D &end, Function *function);
	inline bool pointInWedge(const Point2D &point, int right, int left)
	{
		return g[right - 1](point) >= 0 && g[left - 1](point) < 0;
	}

	int vertexCount;
	Function *f;
	Function *g;
	Point2D origin;
};

