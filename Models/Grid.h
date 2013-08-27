#ifndef GRID_H
#define GRID_H

#include <vector>
#include <stdio.h>
#include "Tetromino.h"
#include "../lib/Angel.h"

using namespace std;

struct Block {
	vec4 color;
};

class Grid {
public:

	//Creates a 3D grid with the size of x, y, z
	Grid(int row, int col, int depth);

	//Checks the grid for rows that have a block in every column position.
	//Removes all full rows and returns the number of rows removed.
	int checkForFullRows();

	//Checks if the specified position contains a block
	bool isPositionFilled(int row, int col, int depth);

	//Returns the total amount of blocks in the grid
	int getNumOfBlocks();

	//Checks if the specified row has a block in every column position
	bool isRowFilled(int row);

	//Prints the grid to console
	void printGrid();

	//Adds a shape to the vector
	void addBlocks(vec4 points[], vec4 color, int size);

	//Checks if blocks can be added
	bool canAddBlocks(vec4 points[], int size);
	//Returns the number of rows in the grid
	int getNumOfRows();

	//Returns the number of columns in the grid
	int getNumOfColumns();

	//Deletes all blocks in the grid
	void deleteAllBlocks();

	//Returns the depth of the grid
	int getDepthSize();

	vector< vector < vector <Block*> > > grid;
private:
	int numOfRows;
	int numOfColumns;
	int depthSize;

	//Removes the row at the specified index
	void removeRow(int rowIndex);
	void getMinXY(vec4 points[], int offset, int &row, int &col, int &depth);


};


#endif
