#include "OLogNPolarSubdivisionTest.h"
#include "Geometry.h"

void OLogNPolarSubdivisionTest::preprocess()
{
	const std::deque<Point2D> & vertices = polygon.vertices;

	this->vertexCount = vertices.size();
	this->origin = vertices[0];
	this->f = new Function[vertexCount - 2];
	this->g = new Function[vertexCount - 1];

	for (int i = 1; i < vertexCount - 1; i++)
	{
		prepareImplicitLine(vertices[i], vertices[i + 1], &(this->f[i - 1]));
		prepareImplicitLine(origin, vertices[i], &(this->g[i - 1]));
	}

	prepareImplicitLine(origin, vertices[vertexCount - 1], &(this->g[vertexCount - 2]));
}

void OLogNPolarSubdivisionTest::deinit()
{
	delete[] f;
	delete[] g;
}

bool OLogNPolarSubdivisionTest::testPoint(const Point2D & point)
{
	int right = 1;
	int left = vertexCount - 1;

	if (!pointInWedge(point, right, left)) {
		return false;
	}

	while ((left - right) != 1) {
		int split = (right + left) / 2;

		if (pointInWedge(point, right, split))
		{
			left = split;
		}
		else
		{
			right = split;
		}
	}

	Function f = this->f[right - 1];

	return f(point) >= 0;
}

void OLogNPolarSubdivisionTest::prepareImplicitLine(const Point2D & start, const Point2D & end, Function * function)
{
	Geometry::getImplicitLine(start, end, &(function->a), &(function->b), &(function->c));
}

