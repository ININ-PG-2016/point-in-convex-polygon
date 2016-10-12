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
	
	inline double dot(const Point2D &other) const;
	inline Point2D normal(Direction d) const;
	inline double length();
	inline double lengthSquare();

	//operators
	inline Point2D operator-();
	inline Point2D operator+(const Point2D &other);
	inline Point2D operator-(const Point2D &other);
	inline Point2D operator*(double value);
	inline Point2D operator/(double value);
	inline void operator+=(const Point2D &other);
	inline void operator-=(const Point2D &other);
	inline void operator*=(double value);
	inline void operator/=(double value);

	inline friend Point2D operator*(double value, const Point2D &point);
	inline friend Point2D operator/(double value, const Point2D &point);

public:
	double x, y;
};

