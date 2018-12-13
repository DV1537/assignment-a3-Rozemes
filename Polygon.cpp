#include "Polygon.h"

Polygon Polygon::operator+(const Polygon& other)
{
	int newSize;
	Coord *temp = this->addVertices(other, newSize);

	for (int i = 0; i < newSize; i++)
	{
		cout << "(" << temp[i].x << ", " << temp[i].y << ")" << endl;
	}
	Polygon newPolygon(temp, newSize);
	delete[] temp;

	return newPolygon;
}
