#include "Geometry.h"

void Geometry::getImplicitLine(const Point2D & p1, const Point2D & p2, double* aOut, double* bOut, double* cOut)
{
	Point2D normalVector = (p2 - p1).normal(Direction::LEFT);
	*aOut = normalVector.x;
	*bOut = normalVector.y;
	*cOut = -(*aOut) * p1.x - (*bOut) * p1.y;
}

double Geometry::getLineIntersectionParameter(double a, double b, double c, const Point2D & p1, const Point2D & p2)
{
	Point2D normal(a, b);
	return (-c - normal.dot(p1)) / normal.dot(p2 - p1);
}