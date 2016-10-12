#pragma once
class Point2D;

enum Direction {
	LEFT, RIGHT
};

class Point2D {
public:
	Point2D();
	Point2D(double x, double y);
	~Point2D();
	
	double dot(const Point2D &other) const;
	Point2D normal(Direction d) const;
	double length() const;
	double lengthSquare() const;

	//operators
	Point2D operator-() const;
	Point2D operator+(const Point2D &other) const;
	Point2D operator-(const Point2D &other) const;
	Point2D operator*(double value) const;
	Point2D operator/(double value) const;
	void operator+=(const Point2D &other);
	void operator-=(const Point2D &other);
	void operator*=(double value);
	void operator/=(double value);

	friend Point2D operator*(double value, const Point2D &point);
	friend Point2D operator/(double value, const Point2D &point);

public:
	double x, y;
};

