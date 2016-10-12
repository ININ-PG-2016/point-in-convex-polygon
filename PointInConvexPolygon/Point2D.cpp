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

double Point2D::lengthSquare() const
{
	return dot(*this);
}

double Point2D::length() const
{
	return sqrt(lengthSquare());
}

Point2D Point2D::operator-() const
{
	return Point2D(-this->x, -this->y);
}

Point2D Point2D::operator+(const Point2D & other) const
{
	return Point2D(this->x + other.x, this->y + other.y);
}

Point2D Point2D::operator-(const Point2D & other) const
{
	return Point2D(this->x - other.x, this->y - other.y);
}

Point2D Point2D::operator*(double value) const
{
	return Point2D(this->x * value, this->y * value);
}

Point2D Point2D::operator/(double value) const
{
	return  (*this) * (1.0 / value);
}

void Point2D::operator+=(const Point2D & other)
{
	this->x += other.x;
	this->y += other.y;
}

void Point2D::operator-=(const Point2D & other)
{
	this->x -= other.x;
	this->y -= other.y;
}

void Point2D::operator*=(double value)
{
	this->x *= value;
	this->y *= value;
}

void Point2D::operator/=(double value)
{
	(*this) *= (1.0 / value);
}

Point2D operator*(double value, const Point2D & point)
{
	return Point2D(point.x * value, point.y * value);
}

Point2D operator/(double value, const Point2D & point)
{
	return (1 / value) * point;
}
