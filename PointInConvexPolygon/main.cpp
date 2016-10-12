#include <iostream>
#include <time.h>
#include "Point2D.h"
#include "Geometry.h"

int main(int argc, char **argv)
{
	/*Point2D p1(1, 2);
	Point2D p2(2, 1);
	Point2D p3 = p1 + p2;
	p3 *= 2;
	p3 += Point2D(2, 4);
	p3 = p3.normal(Direction::LEFT);
	std::cout << p3.x << " " << p3.y <<std::endl;
	std::cout << (p3 / 2).dot(Point2D(2, 3)) << std::endl;
	std::cin.get();*/

	Point2D p1(2, 0);
	Point2D p2(3, 1);
	Point2D p3(-3, 7);
	Point2D p4(6, -2);
	double a;
	double b;
	double c;
	Geometry::getImplicitLine(p3, p4, &a, &b, &c);
	double t;
	long time = clock();
	for (int i = 0; i < 1000000000; i++)
		t = Geometry::getLineIntersectionParameter(a, b, c, p1, p2);
	time = clock() - time;
	std::cout << t << " " << time;
	std::cin.get();
	return 0;
}