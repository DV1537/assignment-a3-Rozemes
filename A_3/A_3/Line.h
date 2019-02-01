#ifndef LINE
#define LINE

#include "Shape.h"

class Line : public Shape
{
public:
	Line(Coord *vertices, int size) : Shape(vertices, size) {};
	~Line() {};

	string getType() const override { return "Line"; };

};

#endif