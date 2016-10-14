#pragma once
#include <deque>

#include "Point2D.h"

class Polygon
{
public:
	Polygon();
	Polygon(int vertexCount);
	Polygon(const std::deque<Point2D> &vertices);
	~Polygon();
	bool saveToFile(char *filename);
public:
	std::deque<Point2D> vertices;

private:
	void loadFromFile(char *filename);
	void generate(int numVertices);
};

