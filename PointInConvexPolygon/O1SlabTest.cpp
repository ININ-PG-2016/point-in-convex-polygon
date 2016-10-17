#include "O1SlabTest.h"
#include "Geometry.h"

#include <iostream>

void O1SlabTest::preprocess()
{
	const std::vector<Point2D> & vertices = polygon.vertices;
	this->vertexCount = vertices.size();

	std::vector<SimpleSlab> simpleSlabs;

	prepareFunctions();
	prepareSimpleSlabs(simpleSlabs);
	computeDistances(simpleSlabs);

	this->slabs = new std::vector<int>[this->slabCount];

	for (int i = 0; i < simpleSlabs.size(); i++)
	{
		SimpleSlab simpleSlab = simpleSlabs[i];

		double top = (0 == i) ? yMax : simpleSlabs[i - 1].y;
		double bottom = simpleSlab.y;

		int indexTop = getSlabIndex(top);
		int indexBottom = getSlabIndex(bottom);

		for (int i = indexTop; i <= indexBottom; i++)
		{
			slabs[i].push_back(simpleSlab.i);
			slabs[i].push_back(simpleSlab.j);
		}

	}
}

void O1SlabTest::deinit()
{
	delete[] slabs;
	delete[] functions;
}

bool O1SlabTest::testPoint(const Point2D & point)
{
	if (point.y < yMin || point.y > yMax)
	{
		return false;
	}

	int slabIndex = getSlabIndex(point.y);

	for (int i = 0; i < slabs[slabIndex].size(); i++)
	{
		if ((functions[slabs[slabIndex][i]])(point) < 0)
		{
			return false;
		}
	}

	return true;
}

void O1SlabTest::prepareSimpleSlabs(std::vector<SimpleSlab>& simpleSlabs)
{
	const std::vector<Point2D> & vertices = polygon.vertices;

	yMax = -HUGE_VAL;
	yMin = HUGE_VAL;

	for (int i = 0; i < vertexCount; i++)
	{
		double y = vertices[i].y;
		if (y < yMin)
		{
			yMinIndex = i;
			yMin = y;
		}

		if (y > yMax)
		{
			yMaxIndex = i;
			yMax = y;
		}
	}

	int iMin = yMinIndex; int jMin = yMinIndex;
	int iMax = yMaxIndex; int jMax = yMaxIndex;

	if (vertices[(iMax + 1) % vertexCount].y == yMax)
	{
		iMax = (iMax + 1) % vertexCount;
	}
	else if (vertices[(jMax + vertexCount - 1) % vertexCount].y == yMax)
	{
		jMax = (jMax + vertexCount - 1) % vertexCount;
	}

	if (vertices[(iMin + vertexCount - 1) % vertexCount].y == yMin)
	{
		iMin = (iMin + vertexCount - 1) % vertexCount;
	}
	else if (vertices[(jMin + 1) % vertexCount].y == yMin)
	{
		jMin = (jMin + 1) % vertexCount;
	}

	int i = iMax; int j = jMax;
	int iNext = (iMax + 1) % vertexCount; int jNext = (jMax + vertexCount - 1) % vertexCount;

	while (i != iMin || j != jMin)
	{
		SimpleSlab slab;

		double yINext = vertices[iNext].y;
		double yJNext = vertices[jNext].y;

		slab.i = i;
		slab.j = jNext;

		if (yINext > yJNext)
		{
			slab.y = yINext;

			i = iNext;
			iNext = (i + 1) % vertexCount;
		}
		else if (yINext < yJNext)
		{
			slab.y = yJNext;

			j = jNext;
			jNext = (j + vertexCount - 1) % vertexCount;
		}
		else
		{
			slab.y = yINext;

			i = iNext;
			iNext = (i + 1) % vertexCount;

			j = jNext;
			jNext = (j + vertexCount - 1) % vertexCount;
		}

		simpleSlabs.push_back(slab);
	}
}

void O1SlabTest::computeDistances(const std::vector<SimpleSlab>& simpleSlabs)
{
	double minDist = yMax - simpleSlabs[0].y;
	double maxDist = yMax - yMin;

	for (int i = 0; i < simpleSlabs.size() - 1; i++)
	{
		double y = simpleSlabs[i].y;
		double nextY = simpleSlabs[i + 1].y;

		double dist = abs(y - nextY);

		if (dist < minDist)
		{
			minDist = dist;
		}
	}


	double minAllowedDist = maxDist / (double)maxSlabCount;

	this->minDist = (minDist > minAllowedDist) ? minDist : minAllowedDist;
	this->minDistInv = 1 / this->minDist;
	this->maxDist = maxDist;
	this->slabCount = ((int)(this->maxDist / this->minDist)) + 1;
}

void O1SlabTest::prepareFunctions()
{
	const std::vector<Point2D> & vertices = polygon.vertices;

	this->functions = new Function[vertexCount];

	for (int i = 0; i < vertexCount - 1; i++)
	{
		prepareImplicitLine(vertices[i], vertices[i + 1], &(this->functions[i]));
	}

	Point2D last = vertices[vertexCount - 1];
	Point2D first = vertices[0];

	prepareImplicitLine(vertices[vertexCount - 1], vertices[0], &(this->functions[vertexCount - 1]));
}

int O1SlabTest::getSlabIndex(double y)
{
	return (yMax - y) * minDistInv;
}

void O1SlabTest::prepareImplicitLine(const Point2D & start, const Point2D & end, Function * function)
{
	Geometry::getImplicitLine(start, end, &(function->a), &(function->b), &(function->c));
}
