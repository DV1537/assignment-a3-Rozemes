#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "Shape.h"
#include "Polygon.h"
#include "Triangle.h"
#include "Line.h"
#include "Point.h"

using namespace std;

enum errorMessage {
	fileIsEmpty,
	fileNotFound,
	wrongInputSyntax,
	validFile
};
Shape **createArray(string fileName, int &size);
errorMessage tryAndReadFile(string fileName, int argc);
float roundDecimals(float nmbr, int decimals);
int nmbrOfWordsInStr(string s);
bool strHasNmbrsOnly(string s);

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Shape **shapeList = nullptr;
	int size;

	if (argc > 1)
	{
		switch (tryAndReadFile(argv[1], argc))
		{
		case fileNotFound:
			cout << "The given argument is not an existing file" << endl;
			system("pause");
			exit(EXIT_FAILURE);
			break;
		case wrongInputSyntax:
			cout << "The file contains invalid data" << endl;
			system("pause");
			exit(EXIT_FAILURE);
			break;
		case fileIsEmpty:
			cout << "The file is empty" << endl;
			system("pause");
			exit(EXIT_FAILURE);
			break;
		case validFile:

			Polygon *sum = new Polygon;
			*sum = (*shapeList[0] + *shapeList[1]);

			cout << *shapeList[0] << endl;
			cout << *shapeList[1] << endl;
			cout << *sum << endl;

			// Freeing memory at the end
			delete shapeList[0];
			delete shapeList[1];
			delete sum;
			delete[] shapeList;
		}
	}
	else
		cout << "No second argument to read file was given" << endl;


	system("pause");
	return 0;
}

Shape **createArray(string fileName, int &size) {

	int nrOfLines = 0, nrOfNmbrsInLine, currentRow = 0;

	Shape **tempShapeArr = nullptr;
	Coord *tempCoordArr = nullptr;
	string line, nmbrInLine;
	ifstream file;
	file.open(fileName);

	// counts the amount of rows (polygons) in the file and ignores the empty lines
	while (getline(file, line)) {
		if (line.compare("\n") > 0)
			nrOfLines++;
	}
	size = nrOfLines;
	tempShapeArr = new Shape*[nrOfLines];

	file.clear();
	file.seekg(0);

	while (currentRow < 2) {

		getline(file, line);

		// Below the function counts the amount of numbers at the current row
		nrOfNmbrsInLine = nmbrOfWordsInStr(line);

		if (nrOfNmbrsInLine > 0) {

			// We allocate memory for a temporary Coord list and fill it with values of the current row/line
			tempCoordArr = new Coord[nrOfNmbrsInLine / 2];

			nmbrInLine = "";
			int count = 0;
			int index = 0;
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] != ' ')
					nmbrInLine += line[i];

				if (line[i] == ' ' && nmbrInLine.compare("") != 0 || i == line.size() - 1 && nmbrInLine.compare("") != 0)
				{
					if (count % 2 == 0)
						tempCoordArr[index].x = stof(nmbrInLine);
					else {
						tempCoordArr[index].y = stof(nmbrInLine);
						index++;
					}
					count++;
					nmbrInLine = "";
				}
			}

			// Allocates memory for the current row to a Polygon just for the simplicity
			
			tempShapeArr[currentRow] = new Polygon(tempCoordArr, nrOfNmbrsInLine / 2);
				

			delete[] tempCoordArr;
			currentRow++;
		}
	}

	file.close();

	return tempShapeArr;
}

errorMessage tryAndReadFile(string fileName, int argc) {

	int size = 0;
	errorMessage status;
	ifstream myReadFile;
	string line;


	myReadFile.open(fileName);

	if (myReadFile.fail())
		status = fileNotFound;
	else
	{
		bool validData = true;

		// check for errors in each row until it finds an something wrong with the syntax
		while (getline(myReadFile, line) && validData)
		{
			size++;

			// if the data is invalid then set status as "wrongInputSyntax" and stop reading the 
			if (!strHasNmbrsOnly(line) || nmbrOfWordsInStr(line) % 2 == 1) {
				validData = false;
				status = wrongInputSyntax;
			}
		}

		// If the size is equal to zero then the file is empty
		if (validData)
		{
			if (size == 0)
				status = fileIsEmpty;
			else
				status = validFile;
		}
	}

	myReadFile.close();

	return status;
}

float roundDecimals(float nmbr, int decimals) {

	nmbr *= pow(10, decimals);
	nmbr = floor(nmbr);
	nmbr /= pow(10, decimals);

	return nmbr;
}

int nmbrOfWordsInStr(string s)
{
	// The function only counts the amount of words in a string that are seperated by spaces and not new lines
	int nmbrOfWords = 0;
	string nmbrInLine = "";

	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] != ' ')
			nmbrInLine += s[i];

		if (nmbrInLine != "" && (s[i] == ' ' || i == s.size() - 1))
		{
			nmbrOfWords++;
			nmbrInLine = "";
		}
	}
	return nmbrOfWords;
}

bool strHasNmbrsOnly(string s)
{
	// if any word in the string can't be interpreted as a number using the stof() function then the function will return false 

	string nmbrInLine = "";
	bool invalidStr = false;

	for (int i = 0; i < s.size() && !invalidStr; i++)
	{
		if (s[i] != ' ')
			nmbrInLine += s[i];

		if (nmbrInLine != "" && (s[i] == ' ' || i == s.size() - 1))
		{
			try
			{
				stof(nmbrInLine);
			}
			catch (const std::exception&)
			{
				invalidStr = true;
			}
			nmbrInLine = "";
		}
	}

	return !invalidStr;
}