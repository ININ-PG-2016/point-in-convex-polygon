#include "OLogNSlabTest.h"
#include "Geometry.h"

//#include <iostream>

void OLogNSlabTest::preprocess()
{
	const std::deque<Point2D> & vertices = polygon.vertices;
	maxYIndex = 0;
	minYIndex = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].y > vertices[maxYIndex].y)
			maxYIndex = i;
		if (vertices[i].y < vertices[minYIndex].y)
			minYIndex = i;
	}
	slabs = new Slab[vertices.size() - 1];
	int leftIndex = minYIndex;
	int rightIndex = minYIndex;
	int nextLeft = leftIndex - 1;
	int nextRight = rightIndex + 1;
	double nextBottom = vertices[minYIndex].y;
	for (int i = 0; i < vertices.size() - 1; i++)
	{
		if (nextLeft < 0)
			nextLeft = (int)vertices.size() - 1;
		if (nextRight >= vertices.size())
			nextRight = 0;
		slabs[i].yBottom = nextBottom;
		Geometry::getImplicitLine(vertices[nextLeft], vertices[leftIndex], &slabs[i].f1.a,
			&slabs[i].f1.b, &slabs[i].f1.c);
		Geometry::getImplicitLine(vertices[rightIndex], vertices[nextRight], &slabs[i].f2.a,
			&slabs[i].f2.b, &slabs[i].f2.c);
		if (vertices[nextLeft].y < vertices[nextRight].y)
		{
			nextBottom = vertices[nextLeft].y;
			leftIndex = nextLeft;
			nextLeft--;
		}
		else
		{
			nextBottom = vertices[nextRight].y;
			rightIndex = nextRight;
			nextRight++;
		}
	}
	/*for (int i = 0; i < vertices.size() - 1; i++)
	{
		std::cout << slabs[i].yBottom <<
			" f1: " << slabs[i].f1.a << ", " << slabs[i].f1.b << ", " << slabs[i].f1.c <<
			" f2: " << slabs[i].f2.a << ", " << slabs[i].f2.b << ", " << slabs[i].f2.c << std::endl;
	}*/
}

void OLogNSlabTest::deinit()
{
	delete[] slabs;
}

bool OLogNSlabTest::testPoint(const Point2D & point)
{
	return true;
}