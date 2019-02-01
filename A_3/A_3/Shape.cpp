#include "Shape.h"

Shape::Shape(Coord  *vertices, int size)
	: size(size), vertices(vertices)
{
	if (size > 0)
		this->vertices = new Coord[size];

	for (int i = 0; i < this->size; i++)
		this->vertices[i] = vertices[i];
}

Shape::~Shape()
{
	this->freeMemory();
}

float Shape::area() const
{
	float area = -1;

	// If NOT self-intersecting and NOT concave(CONVEX) then calculate the area
	if (this->isCovex() && !this->isSelfIntersecting())
	{
		float sum = 0;

		for (int i = 0; i < this->size; i++)
			sum += (this->vertices[i].x * this->vertices[(i + 1) % this->size].y) - (this->vertices[i].y * this->vertices[(i + 1) % this->size].x);

		area = abs(sum) / 2;
	}

	if (area == 0)
		area = -1;

	return area;
}

float Shape::circumreference() const
{
	// Add all the hypotenuses of each edge in a variable and POFF there's your circumreference
	float circumreference = 0;
	float deltaX;
	float deltaY;

	for (int i = 0; i < size; i++)
	{
		deltaX = this->vertices[i].x - this->vertices[(i + 1) % this->size].x;
		deltaY = this->vertices[i].y - this->vertices[(i + 1) % this->size].y;

		circumreference += hypot(deltaX, deltaY);
	}

	return circumreference;
}

Coord Shape::position() const
{
	// Find the max and min x and y values then divide them by two and POFF there's your centerpoint of the polygons boundingBox
	Coord centerPos;
	float xMax = this->vertices[0].x, xMin = this->vertices[0].x;
	float yMax = this->vertices[0].y, yMin = this->vertices[0].y;

	for (int i = 1; i < this->size; i++)
	{
		if (this->vertices[i].x > xMax)
			xMax = this->vertices[i].x;

		if (this->vertices[i].x < xMin)
			xMin = this->vertices[i].x;

		if (this->vertices[i].y > yMax)
			yMax = this->vertices[i].y;

		if (this->vertices[i].y < yMin)
			yMin = this->vertices[i].y;
	}
	centerPos.x = ((xMax - xMin) / 2) + xMin;
	centerPos.y = ((yMax - yMin) / 2) + yMin;

	return centerPos;
}

bool Shape::isCovex() const
{
	bool polygonIsConvex = true;
	bool positive = false, negative = false;
	float Ax, Ay, Bx, By, crossProduct;

	for (int i = 0; i < this->size && polygonIsConvex; i++)
	{
		Ax = this->vertices[i].x - this->vertices[(i + 1) % this->size].x;
		Ay = this->vertices[i].y - this->vertices[(i + 1) % this->size].y;

		Bx = this->vertices[(i + 2) % this->size].x - this->vertices[(i + 1) % this->size].x;
		By = this->vertices[(i + 2) % this->size].y - this->vertices[(i + 1) % this->size].y;

		crossProduct = (Ax * By - Ay * Bx);

		if (crossProduct < 0)
			negative = true;
		else if (crossProduct > 0)
			positive = true;

		if (positive && negative)
			polygonIsConvex = false;
	}

	return polygonIsConvex;
}

float Shape::distance(const Shape & s) const
{
	float deltaX = s.position().x - this->position().x;
	float deltaY = s.position().y - this->position().y;

	float distance = hypot(deltaX, deltaY);

	return distance;
}

void Shape::freeMemory()
{
	delete[] this->vertices;
	this->vertices = nullptr;
	this->size = 0;
}

void Shape::copy(const Shape & other)
{

	this->size = other.size;

	this->vertices = new Coord[other.size];
	for (int i = 0; i < other.size; i++)
		this->vertices[i] = other.vertices[i];

}

Shape& Shape::operator=(const Shape & other)
{
	if (this != &other) {

		this->freeMemory();
		this->copy(other);
	}
	return *this;
}


ostream& operator<<(std::ostream& os, const Shape& other) {

	os << other.getType() << " = {";
	for (int i = 0; i < other.size; i++) {
		os << "(" << other.vertices[i].x << ", " << other.vertices[i].y << ")";
		if (i != other.size - 1)
			os << ", ";
	}
	os << "}" << endl;

	return os;
}

bool Shape::isSelfIntersecting() const
{
	float k1, k2, m1, m2, deltaX, deltaY, intersectingY, intersectingX;
	bool k1xPosDir, k1yPosDir, k2xPosDir, k2yPosDir, k1IsVertical, k1IsHorizontal, k1IsDiagonal, k2IsVertical, k2IsHorizontal, k2IsDiagonal;

	bool validIntersectionFound = false;
	bool allPointAreCollinear = true;

	float previousK1;
	bool previousK1IsVertical;
	bool previousK1IsHorizontal;

	// Loops between every line segment that the points are forming to find an intersection
	for (int i = 0; i < this->size && !validIntersectionFound && this->size > 2; i++)
	{
		if (i > 0)
		{
			previousK1 = k1;
			previousK1IsVertical = k1IsVertical;
			previousK1IsHorizontal = k1IsHorizontal;
		}

		k1 = 0;
		k1IsVertical = false;
		k1IsHorizontal = false;
		k1IsDiagonal = false;

		deltaY = (this->vertices[(i + 1) % this->size].y - this->vertices[i].y);
		deltaX = (this->vertices[(i + 1) % this->size].x - this->vertices[i].x);

		if (deltaY == 0)
		{
			intersectingY = this->vertices[i].y;
			k1IsHorizontal = true;
		}
		else if (deltaX == 0)
		{
			intersectingX = this->vertices[i].x;
			k1IsVertical = true;
		}
		else
		{
			k1 = deltaY / deltaX;
			m1 = this->vertices[i].y - k1 * this->vertices[i].x;
			k1IsDiagonal = true;
		}
		// check if all the lines are collinear, then it's self-intersecting
		if (i > 0)
			if (k1 != previousK1 || k1IsHorizontal != previousK1IsHorizontal || k1IsVertical != previousK1IsVertical)
				allPointAreCollinear = false;

		// Checks which direction the line (i and i+1) is moving in, positive or negative  in both x and y-axis
		if (this->vertices[i].x < this->vertices[(i + 1) % this->size].x)
			k1xPosDir = true;
		else
			k1xPosDir = false;

		if (this->vertices[i].y < this->vertices[(i + 1) % this->size].y)
			k1yPosDir = true;
		else
			k1yPosDir = false;


		// Check if the line that the points i and i+1 forms intersects with the lines ahead of it
		for (int j = i + 2; j < (this->size + i - 1) && !validIntersectionFound; j++)
		{
			k2 = 0;
			k2IsVertical = false;
			k2IsHorizontal = false;
			k2IsDiagonal = false;

			deltaY = (this->vertices[(j + 1) % this->size].y - this->vertices[j % this->size].y);
			deltaX = (this->vertices[(j + 1) % this->size].x - this->vertices[j % this->size].x);

			if (deltaY == 0)
			{
				intersectingY = this->vertices[j % this->size].y;
				k2IsHorizontal = true;
			}
			else if (deltaX == 0)
			{
				intersectingX = this->vertices[j % this->size].x;
				k2IsVertical = true;
			}
			else
			{
				k2 = deltaY / deltaX;
				m2 = this->vertices[j % this->size].y - k2 * this->vertices[j % this->size].x;
				k2IsDiagonal = true;
			}

			// Checks which direction the line (i and i+1) is moving in, positive or negative  in both x and y-axis
			if (this->vertices[j % this->size].x < this->vertices[(j + 1) % this->size].x)
				k2xPosDir = true;
			else
				k2xPosDir = false;

			if (this->vertices[j % this->size].y < this->vertices[(j + 1) % this->size].y)
				k2yPosDir = true;
			else
				k2yPosDir = false;

			// Using the linear equation to find which x and y coordinates the two lines share, that coordinate will be the intersection of the lines

			if (k1IsHorizontal && k2IsDiagonal)
			{
				intersectingX = (intersectingY - m2) / k2;
			}
			else if (k1IsVertical && k2IsDiagonal)
			{
				intersectingY = k2 * intersectingX + m2;
			}
			else if (k1IsDiagonal && k2IsHorizontal)
			{
				intersectingX = (intersectingY - m1) / k1;
			}
			else if (k1IsDiagonal && k2IsVertical)
			{
				intersectingY = k1 * intersectingX + m1;
			}
			else if (k1IsDiagonal && k2IsDiagonal)
			{
				intersectingX = (m2 - m1) / (k1 - k2);
				intersectingY = k1 * intersectingX + m1;
			}

			// If both lines are parallel then there is no intersection between them and the code inside the if-statement should then be igonored
			if (!(k1IsHorizontal && k2IsHorizontal || k1IsVertical && k2IsVertical || k1 == k2 && k1 != 0 && k2 != 0))
			{
				bool validX = false;
				bool validY = false;

				// check if the intersecting X value is between the right interval
				if (k1xPosDir && intersectingX >= this->vertices[i].x && intersectingX <= this->vertices[(i + 1) % this->size].x)
					validX = true;
				else if (!k1xPosDir && intersectingX <= this->vertices[i].x && intersectingX >= this->vertices[(i + 1) % this->size].x)
					validX = true;

				// check if the intersecting Y value is between the right interval
				if (k1yPosDir && intersectingY >= this->vertices[i].y && intersectingY <= this->vertices[(i + 1) % this->size].y)
					validY = true;
				else if (!k1yPosDir && intersectingY <= this->vertices[i].y && intersectingY >= this->vertices[(i + 1) % this->size].y)
					validY = true;

				// -------------------------------------------------

				bool validX2 = false;
				bool validY2 = false;

				// check if the intersecting X value is between the right interval
				if (k2xPosDir && intersectingX >= this->vertices[j%this->size].x && intersectingX <= this->vertices[(j + 1) % this->size].x)
					validX2 = true;
				else if (!k2xPosDir && intersectingX <= this->vertices[j%this->size].x && intersectingX >= this->vertices[(j + 1) % this->size].x)
					validX2 = true;

				// check if the intersecting Y value is between the right interval
				if (k2yPosDir && intersectingY >= this->vertices[j%this->size].y && intersectingY <= this->vertices[(j + 1) % this->size].y)
					validY2 = true;
				else if (!k2yPosDir && intersectingY <= this->vertices[j%this->size].y && intersectingY >= this->vertices[(j + 1) % this->size].y)
					validY2 = true;

				// If both intersectingX and intersectingY are valid intersections within the line segments then the polygon is self-intersecting
				if (validY && validX && validX2 && validY2)
					validIntersectionFound = true;
			}
		}
	}
	if (allPointAreCollinear && this->size > 2)
		validIntersectionFound = true;

	return validIntersectionFound;
}
