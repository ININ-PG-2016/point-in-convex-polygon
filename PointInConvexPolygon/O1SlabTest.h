#pragma once
#include "PointInConvexPolygonTest.h"

#include <vector>

class O1SlabTest :
	public PointInConvexPolygonTest
{
public:
	O1SlabTest(const class Polygon & p) : PointInConvexPolygonTest(p), maxSlabCount(p.vertices.size()) {}
	void preprocess();
	void deinit();
	bool testPoint(const Point2D & point);
	void setMaxSlabCount(int maxSlabCount)
	{
		this->maxSlabCount = maxSlabCount;
	}

private:
	double yMin;
	double yMax;
	int yMinIndex;
	int yMaxIndex;

	double minDist;
	double minDistInv;
	double maxDist;

	struct SimpleSlab
	{
		double y;
		int i;
		int j;
	};

	int vertexCount;
	int slabCount;
	int maxSlabCount;
	std::vector<int> *slabs;
	Function *functions;

	void prepareSimpleSlabs(std::vector<SimpleSlab> &simpleSlabs);
	void computeDistances(const std::vector<SimpleSlab> &simpleSlabs);
	void prepareFunctions();
	int getSlabIndex(double y);
	void prepareImplicitLine(const Point2D &start, const Point2D &end, Function *function);
};

