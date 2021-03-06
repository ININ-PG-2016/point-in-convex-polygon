#include "Geometry.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Geometry::getImplicitLine(const Point2D & p1, const Point2D & p2, double* aOut, double* bOut, double* cOut)
{
	Point2D normalVector = (p2 - p1).normal(Direction::LEFT);
	*aOut = normalVector.x;
	*bOut = normalVector.y;
	*cOut = -(*aOut) * p1.x - (*bOut) * p1.y;
	if (*cOut == 0) {
		std::cout.precision(30);
		std::cout << p1.x << ", " << p1.y << std::endl;
		std::cout << p2.x << ", " << p2.y << std::endl << std::endl;
	}
}

double Geometry::getLineIntersectionParameter(double a, double b, double c, const Point2D & p1, const Point2D & p2)
{
	Point2D normal(a, b);
	return (-c - normal.dot(p1)) / normal.dot(p2 - p1);
}

bool Geometry::loadPointsFromFile(char * filename, std::vector<Point2D>& points)
{
	std::ifstream in(filename);
	in.precision(30);

	//Could not open the stream
	if (!in)
	{
		std::cout << "File read error!" << std::endl;
		return false;
	}

	char line[255];

	while (in.good())
	{
		in.getline(line, 255);

		if (in)
		{
			std::vector<double> values;
			char *token = strtok(line, " ");

			while (token != NULL)
			{
				if (strcmp(token, "") == 0)
				{
					token = strtok(NULL, " ");
					continue;
				}

				std::stringstream tokenstream(token);
				tokenstream.precision(30);

				double tmp;
				tokenstream >> tmp;
				values.push_back(tmp);

				token = strtok(NULL, " ");
			}

			if (values.size() != 2)
			{
				std::cout << "Invalid data!" << std::endl;
				return false;
			}

			points.push_back(Point2D(values[0], values[1]));
		}
	}

	std::cout << "Loaded " << points.size() << " points" << std::endl;

	in.close();
	return true;
}
