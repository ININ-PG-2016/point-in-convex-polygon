#include "CrossProductTest.h"

void CrossProductTest::preprocess()
{
	const std::vector<Point2D> & vertices = polygon.vertices;
	edgeVectors = new Point2D[vertices.size()];
	for (int i = 0; i < vertices.size() - 1; i++)
		edgeVectors[i] = vertices[i + 1] - vertices[i];
	edgeVectors[vertices.size() - 1] = vertices[0] - vertices[vertices.size() - 1];
}

void CrossProductTest::deinit()
{
	delete[] edgeVectors;
}

bool CrossProductTest::testPoint(const Point2D & point)
{
	const std::vector<Point2D> & vertices = polygon.vertices;
	int size = vertices.size();
	for (int i = 0; i < size; i++)
	{
		Point2D pointVector = point - vertices[i];
		if (edgeVectors[i].x * pointVector.y - edgeVectors[i].y * pointVector.x < 0)
			return false;
	}
	return true;
}