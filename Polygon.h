#ifndef POLYGON
#define POLYGON

#include "Shape.h"

class Polygon : public Shape
{
public:
	Polygon(const Coord *vertices, int size) : Shape(vertices, size) {};
	~Polygon() {};

	string getType() const override { return "Polygon"; };

	Polygon operator+(const Polygon& other);


private:

};

#endif
