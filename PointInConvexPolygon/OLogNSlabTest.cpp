#include "OLogNSlabTest.h"
#include "Geometry.h"

#include <iostream>

void OLogNSlabTest::preprocess()
{
	const std::vector<Point2D> & vertices = polygon.vertices;
	maxYIndex = 0;
	minYIndex = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].y > vertices[maxYIndex].y)
			maxYIndex = i;
		if (vertices[i].y < vertices[minYIndex].y)
			minYIndex = i;
	}
	slabCount = (int)vertices.size() - 1;
	slabs = new Slab[slabCount];
	int leftIndex = minYIndex;
	int rightIndex = minYIndex;
	int nextLeft = leftIndex - 1;
	int nextRight = rightIndex + 1;
	double nextBottom = vertices[minYIndex].y;
	for (int i = 0; i < slabCount; i++)
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
	double x = point.x;
	double y = point.y;
	if (y < polygon.vertices[minYIndex].y || y > polygon.vertices[maxYIndex].y)
		return false;
	int topSlab = slabCount;
	int bottomSlab = 0;
	int middleSlab = topSlab / 2;
	while (topSlab - bottomSlab > 1)
	{
		const Slab & slab = slabs[middleSlab];
		if (y > slab.yBottom)
			bottomSlab = middleSlab;
		else
			topSlab = middleSlab;
		middleSlab = (topSlab + bottomSlab) / 2;
	}
	const Slab & slab = slabs[middleSlab];
	if (slab.f1.a * x + slab.f1.b * y + slab.f1.c >= 0 && slab.f2.a * x + slab.f2.b * y + slab.f2.c >= 0)
		return true;
	return false;
}