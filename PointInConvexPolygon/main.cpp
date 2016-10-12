#include <iostream>
#include "Point2D.h"

int main(int argc, char **argv)
{
	Point2D p1(1, 2);
	Point2D p2(2, 1);
	Point2D p3 = p1 + p2;
	p3 *= 2;
	p3 += Point2D(2, 4);
	p3 = p3.normal(Direction::LEFT);
	std::cout << p3.x << " " << p3.y <<std::endl;
	std::cout << (p3 / 2).dot(Point2D(2, 3)) << std::endl;
	std::cin.get();
	return 0;
}