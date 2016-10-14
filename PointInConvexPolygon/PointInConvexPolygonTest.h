#pragma once

#include "Polygon.h"

class PointInConvexPolygonTest
{
protected:
	/*
	represents the function F(x) = ax + by + c
	*/
	struct Function
	{
		double a;
		double b;
		double c;
	};

	const Polygon & polygon;

public:
	PointInConvexPolygonTest(const Polygon & p) : polygon(p) {}
	virtual void preprocess() = 0;
	virtual void deinit() = 0;
	virtual bool testPoint(const Point2D & point) = 0;
};