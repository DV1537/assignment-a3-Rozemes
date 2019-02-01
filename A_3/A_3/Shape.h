#ifndef SHAPE
#define SHAPE
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

struct Coord
{
	float x, y;
};

class Shape
{
public:

	Shape(Coord *vertices, int size);
	virtual ~Shape();

	virtual string getType() const = 0;
	Coord position() const;
	bool isCovex() const;
	bool isSelfIntersecting() const;
	float area() const;
	float circumreference() const;
	float distance(const Shape& s) const;

	void freeMemory();
	void copy(const Shape &other);

	friend ostream& operator<<(ostream& os, const Shape& other);
	Shape& operator=(const Shape &other);

protected:
	int size;
	Coord *vertices;
};

#endif // SHAPE
