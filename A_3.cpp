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
Polygon **createArray(string fileName, int &size);
errorMessage tryAndReadFile(string fileName, int argc);
float roundDecimals(float nmbr, int decimals);

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Polygon **shapeList = nullptr;
	int size;

	if (2 > 1)
	{
		switch (tryAndReadFile("test.txt", 2))
		{
		case fileNotFound:
			cout << "The given argument is not an existing file" << endl;
			break;
		case wrongInputSyntax:
			cout << "The file contains invalid data" << endl;
			break;
		case fileIsEmpty:
			cout << "The file is empty" << endl;
			break;
		case validFile:
			shapeList = createArray("test.txt", size);

			if (shapeList == nullptr)
				cout << "Some rows in file have uneven amounts of numbers in it" << endl;
			else {
				Polygon *sum = shapeList[0];

				// Freeing memory at the end
				for (int i = 0; i < size; i++)
					delete shapeList[i];
			}
			delete[] shapeList;

			break;
		}
	}
	else
		cout << "No second argument to read file was given" << endl;


	system("pause");
	return 0;
}

Polygon **createArray(string fileName, int &size) {

	Polygon **arr = nullptr;
	Coord *temp = nullptr;

	string line, nmbrInLine;
	bool syntaxIsInvalid = false;
	int nrOfLines = 0, nrOfNmbrsInLine, currentRow = 0;

	ifstream file;
	file.open(fileName);

	// counts the amount of rows (polygons) in the file and excludes the empty lines
	while (getline(file, line)) {
		if (line.compare("\n") > 0)
			nrOfLines++;
	}
	size = nrOfLines;
	arr = new Polygon*[nrOfLines];

	file.clear();
	file.seekg(0);

	while (currentRow < 2 && !syntaxIsInvalid) {

		getline(file, line);

		// Below the function counts number of numbers at the current row
		nrOfNmbrsInLine = 0;
		nmbrInLine = "";
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] != ' ')
				nmbrInLine += line[i];

			if (line[i] == ' ' && nmbrInLine.compare("") != 0 || i == line.size() - 1 && nmbrInLine.compare("") != 0)
			{
				nrOfNmbrsInLine++;
				nmbrInLine = "";
			}
		}

		// if the row has NON EVEN amount of numbers then the syntax is wrong and then the memory is freed and we set arr equal to nullptr
		if (nrOfNmbrsInLine % 2 == 1)
		{
			syntaxIsInvalid = true;

			for (int i = 0; i < currentRow; i++)
				delete arr[i];

			delete[] arr;
			arr = nullptr;
		}
		else if (nrOfNmbrsInLine != 0) {

			// We allocate memory for a temporary Coord list and fill it with values of the current row/line
			temp = new Coord[nrOfNmbrsInLine / 2];

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
						temp[index].x = stof(nmbrInLine);
					else {
						temp[index].y = stof(nmbrInLine);
						index++;
					}

					count++;
					nmbrInLine = "";
				}
			}


			arr[currentRow] = new Polygon(temp, nrOfNmbrsInLine / 2);
				
			delete[] temp;
		}

		if (nrOfNmbrsInLine > 0 && !syntaxIsInvalid)
			currentRow++;
	}

	file.close();

	return arr;
}

errorMessage tryAndReadFile(string fileName, int argc) {

	errorMessage status;
	string word;
	int size = 0;

	ifstream myReadFile;
	myReadFile.open(fileName);

	if (myReadFile.fail())
		status = fileNotFound;
	else
	{
		bool validData = true;
		while (myReadFile >> word)
		{
			size++;

			// Compares each characters in "word" with the "allowed characters" 
			// and if a character in "word" doesn't consist of any of the allowed characters then the data is invalid
			string allowedChars = "0123456789.-";
			for (int i = 0; i < word.size() && validData; i++)
			{
				validData = false;
				for (int j = 0; j < allowedChars.size(); j++)
					if (word[i] == allowedChars[j])
						validData = true;
			}
			// if the data is invalid then set status as "wrongInputSyntax"
			if (!validData)
				status = wrongInputSyntax;
		}

		if (validData)
		{
			status = validFile;
			if (size == 0)
				status = fileIsEmpty;
		}
	}
	return status;
}

