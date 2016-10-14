#include "OLogNSlabTest.h"

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
	slabs = new Slab[vertices.size()];

	//to be continued
}

void OLogNSlabTest::deinit()
{
	delete[] slabs;
}

bool OLogNSlabTest::testPoint(const Point2D & point)
{
	return true;
}