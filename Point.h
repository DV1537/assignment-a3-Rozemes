#ifndef POINT
#define POINT

#include "Shape.h"

class Point : public Shape
{
public:
	Point(const Coord *vertices, int size) : Shape(vertices, size) {};
	~Point() {};

	string getType() const override{ return "Point"; };

};

#endif
