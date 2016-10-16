#include "HalfPlaneIntersectionTest.h"
#include "Geometry.h"


void HalfPlaneIntersectionTest::preprocess()
{
	const std::deque<Point2D> & vertices = polygon.vertices;
	
	this->vertexCount = vertices.size();
	this->functions = new Function[vertexCount];

	for (int i = 0; i < vertexCount - 1; i++)
	{
		prepareImplicitLine(vertices[i], vertices[i + 1], &(this->functions[i]));
	}

	Point2D last = vertices[vertexCount - 1];
	Point2D first = vertices[0];

	prepareImplicitLine(vertices[vertexCount - 1], vertices[0], &(this->functions[vertexCount - 1]));
}

void HalfPlaneIntersectionTest::deinit()
{
	delete[] functions;
}

bool HalfPlaneIntersectionTest::testPoint(const Point2D & point)
{
	for (int i = 0; i < vertexCount; i++)
	{
		if (this->functions[i](point) <= 0)
		{
			return false;
		}
	}

	return true;
}

void HalfPlaneIntersectionTest::prepareImplicitLine(const Point2D & start, const Point2D & end, Function * function)
{
	Geometry::getImplicitLine(start, end, &(function->a), &(function->b), &(function->c));
}
