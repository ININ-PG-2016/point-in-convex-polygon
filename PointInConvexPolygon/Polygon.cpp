#include "Polygon.h"
#include "Geometry.h"

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


Polygon::Polygon(const std::vector<Point2D>& vertices)
{
	this->vertices = vertices;
}

Polygon::Polygon(char * filename)
{
	if (!loadFromFile(filename)) {
		this->vertices.clear();
		std::cerr << "Could not read polygon data from file." << std::endl;
	}
}

Polygon::~Polygon()
{}

bool Polygon::saveToFile(char * filename)
{
	std::ofstream out(filename);
	out.precision(30);

	if (!out)
	{
		std::cout << "File write error!" << std::endl;
		return false;
	}

	for (auto vertex : this->vertices)
	{
		out << " " << vertex.x << " " << vertex.y << std::endl;
	}

	out.close();

	std::cout << "Polygon sucessfully saved." << std::endl;
}

bool Polygon::loadFromFile(char * filename)
{
	this->vertices.clear();
	return Geometry::loadPointsFromFile(filename, this->vertices);
}

void Polygon::generate(int numVertices)
{
	srand(time(NULL));

	if (numVertices < 4)
	{
		throw new std::exception("Number of vertices of polygon must be at least 4");
	}

	this->vertices.clear();
	//this->vertices.resize(numVertices);

	this->vertices.push_back(Point2D(-1.0,  0.0));
	this->vertices.push_back(Point2D( 0.0, -1.0));
	this->vertices.push_back(Point2D( 1.0,  0.0));
	this->vertices.push_back(Point2D( 0.0,  1.0));

	int nextIndex = 0;

	double eps = 1.0 / 1000000000000000000000.0;

	for (int i = 0; i < numVertices - 4; i++)
	{
		int currentCount = this->vertices.size();
		if (currentCount % 1000 == 0)
			std::cout << currentCount << std::endl;
		nextIndex = (nextIndex + 2) % currentCount;

		//std::cout << "generated p: " << p1 << ", " << p2 << std::endl;

		Point2D erased = this->vertices[nextIndex];
		Point2D next = this->vertices[(nextIndex + 1) % currentCount];
		Point2D prev = this->vertices[(nextIndex + (currentCount - 1)) % currentCount];

		if ((erased - next).lengthSquare() < eps || (erased - prev).lengthSquare() < eps) {
			i--;
			continue;
		}

		double p1 = ((double)rand() / (RAND_MAX)) * 0.5 + 0.25;
		double p2 = ((double)rand() / (RAND_MAX)) * 0.5 + 0.25;

		Point2D new1 = p1 * erased + ((1 - p1) * prev);
		Point2D new2 = p2 * erased + ((1 - p2) * next);

		std::vector<Point2D> added ({new1, new2});

		this->vertices.erase(this->vertices.begin() + nextIndex);
		this->vertices.insert(this->vertices.begin() + nextIndex, added.begin(), added.end());
	}

	double yMin = HUGE_VAL, yMax = -HUGE_VAL;
	double xMin = HUGE_VAL, xMax = -HUGE_VAL;

	for (int i = 0; i < numVertices; i++)
	{
		Point2D vertex = this->vertices[i];

		if (vertex.x > xMax)
		{
			xMax = vertex.x;
		}

		if (vertex.x < xMin)
		{
			xMin = vertex.x;
		}

		if (vertex.y > yMax)
		{
			yMax = vertex.y;
		}

		if (vertex.y < yMin)
		{
			yMin = vertex.y;
		}
	}

	double deltaX = xMax - xMin;
	double scaleX = 2.0 / deltaX;
	double deltaY = yMax - yMin;
	double scaleY = 2.0 / deltaY;
	double centerX = (xMax + xMin) / 2;
	double centerY = (yMax + yMin) / 2;

	for (int i = 0; i < numVertices; i++)
	{
		this->vertices[i].x = (this->vertices[i].x - centerX) * scaleX;
		this->vertices[i].y = (this->vertices[i].y - centerY) * scaleY;
	}
}
