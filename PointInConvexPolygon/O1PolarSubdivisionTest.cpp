#include "O1PolarSubdivisionTest.h"
#include <math.h>

int O1PolarSubdivisionTest::getWedgeIndex(const Point2D & point)
{
	const Point2D p = point - centroid;
	double absX = abs(p.x);
	double absY = abs(p.y);
	double ratio;
	int index;
	if (absX > absY)
	{
		ratio = absY / absX;
		if (p.x > 0)
			index = (int)(ratio * wedgesPerOctant);
		else
			index = (int)(wedgesPerOctant * 4 - 1 - (double)(ratio * wedgesPerOctant));
	}
	else
	{
		ratio = absX / absY;
		if (p.x > 0)
			index = (int)(wedgesPerOctant * 2 - 1 - (double)(ratio * wedgesPerOctant));
		else
			index = (int)(wedgesPerOctant * 2 + (double)(ratio * wedgesPerOctant));
	}
	if (p.y > 0)
		return index;
	else
		return wedgesPerOctant * 8 - 1 - index;
}

void O1PolarSubdivisionTest::preprocess()
{
	const std::deque<Point2D> & vertices = polygon.vertices;
	int size = vertices.size();
	centroid = Point2D(0, 0);
	double signedArea = 0;
	for (int i = 0; i < vertices.size(); i++)
		signedArea += vertices[i].x * vertices[(i + 1) % size].y - vertices[(i + 1) % size].x * vertices[i].y;
	signedArea /= 2;
	for (int i = 0; i < vertices.size(); i++)
	{
		double mul = (vertices[i].x * vertices[(i + 1) % size].y - vertices[(i + 1) % size].x * vertices[i].y);
		centroid.x += (vertices[i].x + vertices[(i + 1) % size].x) * mul;
		centroid.y += (vertices[i].y + vertices[(i + 1) % size].y) * mul;
	}
	centroid /= (6 * signedArea);

	wedges = new std::vector<Function>[wedgesPerOctant * 8];

	//to be continued
}

void O1PolarSubdivisionTest::deinit()
{
	delete[] wedges;
}

bool O1PolarSubdivisionTest::testPoint(const Point2D & point)
{
	return true;
}