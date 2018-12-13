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

	Shape(const Coord *vertices, int size);
	virtual ~Shape();

	virtual string getType() const = 0;
	Coord position() const;				
	bool isCovex() const;				
	bool isSelfIntersecting() const;	
	float area() const;
	float circumreference() const;
	float distance(const Shape& s) const;

	Coord *addVertices(const Shape& other, int &size);
	void freeMemory();
	void copy(const Shape &other);
	friend std::ostream& operator<<(std::ostream& os, const Shape& other);
	Shape& operator=(const Shape &other);

private:
	int size;
	Coord *vertices;
};


float roundDecimals(float nmbr, int decimals) {

	nmbr *= pow(10, decimals);
	nmbr = floor(nmbr);
	nmbr /= pow(10, decimals);

	return nmbr;
}

#endif // SHAPE
