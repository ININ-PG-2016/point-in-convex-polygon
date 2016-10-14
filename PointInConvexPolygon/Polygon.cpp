#include "Polygon.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>

Polygon::Polygon()
{
	generate(1000);
}

Polygon::Polygon(int vertexCount)
{
	generate(vertexCount);
}


Polygon::Polygon(const std::deque<Point2D>& vertices)
{
	this->vertices = vertices;
}

Polygon::~Polygon()
{}

bool Polygon::saveToFile(char * filename)
{
	std::ofstream out(filename);

	if (!out)
	{
		std::cout << "File write error!" << std::endl;
		return false;
	}

	for (auto vertex : this->vertices)
	{
		out << vertex.x << " " << vertex.y << std::endl;
	}

	out.close();

	std::cout << "Polygon sucessfully saved." << std::endl;
}

void Polygon::loadFromFile(char * filename)
{
	
}

void Polygon::generate(int numVertices)
{
	srand(time(NULL));

	if (numVertices < 4)
	{
		throw new std::exception("Number of vertices of polygon must be at least 4");
	}

	this->vertices.clear();

	this->vertices.push_back(Point2D(-1.0,  0.0));
	this->vertices.push_back(Point2D( 0.0, -1.0));
	this->vertices.push_back(Point2D( 1.0,  0.0));
	this->vertices.push_back(Point2D( 0.0,  1.0));

	int randIndex = 0;

	for (int i = 0; i < numVertices - 4; i++)
	{
		int currentCount = this->vertices.size();

		randIndex = rand() % currentCount;

		double p1 = ((double)rand() / (RAND_MAX)) * 0.5 + 0.25;
		double p2 = ((double)rand() / (RAND_MAX)) * 0.5 + 0.25;

		//std::cout << "generated p: " << p1 << ", " << p2 << std::endl;

		Point2D erased = this->vertices[randIndex];
		Point2D next = this->vertices[(randIndex + 1) % currentCount];
		Point2D prev = this->vertices[(randIndex + (currentCount - 1)) % currentCount];

		Point2D new1 = p1 * erased + ((1 - p1) * prev);
		Point2D new2 = p2 * erased + ((1 - p2) * next);

		std::vector<Point2D> added ({new1, new2});

		this->vertices.erase(this->vertices.begin() + randIndex);
		this->vertices.insert(this->vertices.begin() + randIndex, added.begin(), added.end());
	}
}
