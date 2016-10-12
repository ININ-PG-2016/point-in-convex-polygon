#include "Point2D.h"
#include <math.h>


Point2D::Point2D() : x(0.0), y(0.0) {}

Point2D::Point2D(double x, double y) : x(x), y(y) {}


Point2D::~Point2D() {}

double Point2D::dot(const Point2D & other) const
{
	return this->x * other.x + this->y * other.y;
}

Point2D Point2D::normal(Direction d) const
{
	return (d == LEFT) ? Point2D(-(this->y), this->x) : Point2D(this->y, -(this->x));
}

double Point2D::lengthSquare()
{
	return dot(*this);
}

double Point2D::length()
{
	return sqrt(lengthSquare());
}

inline Point2D Point2D::operator-()
{
	return Point2D(-this->x, -this->y);
}

inline Point2D Point2D::operator+(const Point2D & other)
{
	return Point2D(this->x + other.x, this->y + other.y);
}

inline Point2D Point2D::operator-(const Point2D & other)
{
	return Point2D(this->x - other.x, this->y - other.y);
}

inline Point2D Point2D::operator*(double value)
{
	return Point2D(this->x * value, this->y * value);
}

inline Point2D Point2D::operator/(double value)
{
	return  (*this) * (1.0 / value);
}

inline void Point2D::operator+=(const Point2D & other)
{
	this->x += other.x;
	this->y += other.y;
}

inline void Point2D::operator-=(const Point2D & other)
{
	this->x -= other.x;
	this->y -= other.y;
}

inline void Point2D::operator*=(double value)
{
	this->x *= value;
	this->y *= value;
}

inline void Point2D::operator/=(double value)
{
	(*this) *= (1.0 / value);
}

inline Point2D operator*(double value, const Point2D & point)
{
	return Point2D(point.x * value, point.y * value);
}

inline Point2D operator/(double value, const Point2D & point)
{
	return (1 / value) * point;
}
