#include "Polygon.h"

Polygon& Polygon::operator=(Polygon* other)
{
	if (this != other) {

		this->freeMemory();
		this->copy(*other);
		delete other;
	}
	return *this;
}


Polygon* Polygon::operator+(const Polygon& other)
{
	int newSize = this->size + other.size;
	Coord *newCoord = new Coord[size];

	// adding all the vertices from this & other polygon into the newCoord array
	for (int i = 0; i < newSize; i++)
	{
		if (i < this->size) {
			newCoord[i] = this->vertices[i];
		}
		else {
			newCoord[i] = other.vertices[i - this->size];
		}
	}

	Polygon *newPolygon = new Polygon(newCoord, newSize);

	// freeing memory
	delete[] newCoord;

	return newPolygon;
}

//Polygon operator+(Polygon a, Polygon b)
//{
//	int newSize;
//	Coord *temp = a.addVertices(b, newSize);
//
//	Polygon newPolygon(temp, newSize);
//	delete[] temp;
//
//	return newPolygon;
//}
