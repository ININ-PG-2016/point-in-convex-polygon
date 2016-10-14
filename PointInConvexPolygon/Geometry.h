#pragma once

#include "Point2D.h"

class Geometry
{
public:
	static void getImplicitLine(const Point2D & p1, const Point2D & p2, double* aOut, double* bOut, double* cOut);
	static double getLineIntersectionParameter(double a, double b, double c, const Point2D & p1, const Point2D & p2);
};