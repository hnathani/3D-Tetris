/*
 * Grid.cpp
 *
 *  Created on: 2011-09-22
 *      Author: hna14
 */

#include "Grid.h"
#include <stdio.h>
#define IN_GRID_SPACE(row, col, depth) -1 < (row) && (row) < numOfRows && -1 < (col) && (col) < numOfColumns && -1 < (depth) && (depth) < depthSize

Grid::Grid(int row, int col, int depth) {
	numOfRows = row;
	numOfColumns = col;
	depthSize = depth;

	//Initialize grid
	int r, c, d;
	//Resize the grid vector to the number of rows
	grid.resize(numOfRows);
	//Resize each vector in the rows to the number of columns
	for (r = 0; r < numOfRows; r++) {
		grid[r].resize(numOfColumns);
		for (c = 0; c < numOfColumns; c++) {
			grid[r][c].resize(depthSize);
			for (d = 0; d < depthSize; d++) {
				grid[r][c][d] = NULL;
			}
		}
	}
}

bool Grid::isPositionFilled(int row, int col, int depth) {
	if (IN_GRID_SPACE(row, col, depth)) {
		return (grid[row][col][depth] != NULL);
	}
	return true;
}

int Grid::getNumOfBlocks() {
	int row, col, depth;
	int numOfBlocks = 0;
	//Check each column in every row if a block exists then add to counter
	for (row = 0; row < numOfRows; row++) {
		for (col = 0; col < numOfColumns; col++) {
			for (depth = 0; depth < depthSize; depth++) {
				if (isPositionFilled(row, col, depth)) {
					numOfBlocks++;
				}
			}
		}
	}
	return numOfBlocks;
}

void Grid::addBlocks(vec4 points[], vec4 color, int size) {
	int i;
	for (i = 0; i < size; i = i + T_POINTS_PER_BLOCK) {
		Block *block = new Block;

		block->color[0] = color[0];
		block->color[1] = color[1];
		block->color[2] = color[2];
		block->color[3] = color[3];

		int col = floor(points[i][0] + .5);
		int row = floor(points[i][1] + .5);
		int depth = floor(points[i][2] + .5);
		getMinXY(points, i, row, col, depth);
		grid[row][col][depth] = block;
	}
}

void Grid::getMinXY(vec4 points[], int offset, int &row, int &col, int &depth) {
	int j;
	for (j = 0; j < T_POINTS_PER_BLOCK; j++) {
		if (floor(points[offset + j][1] + .5) < row) {
			row = floor(points[offset + j][1] + .5);
		}

		if (floor(points[offset + j][0] + .5) < col) {
			col = floor(points[offset + j][0] + .5);
		}

		if (floor(points[offset + j][2] + .5) < depth) {
			depth = floor(points[offset + j][2] + .5);
		}

	}
}

bool Grid::canAddBlocks(vec4 points[], int size) {
	int row, col, i, depth;
	for (i = 0; i < size; i = i + T_POINTS_PER_BLOCK) {
		row = floor(points[i][1] + .5);
		col = floor(points[i][0] + .5);
		depth = floor(points[i][2] + .5);
		getMinXY(points, i, row, col, depth);

		if (isPositionFilled(row, col, depth)) {
			return false;
		}
	}
	return true;
}

bool Grid::isRowFilled(int row) {
	int col, depth;
	//Check each column in specified row if there is a block in it
	for (col = 0; col < numOfColumns; col++) {
		for (depth = 0; depth < depthSize; depth++) {
			if (grid[row][col][depth] == NULL) {
				return false;
			}
		}
	}
	return true;
}

int Grid::checkForFullRows() {
	int row;
	int rowsRemoved = 0;
	//Check if each position in the row is full then remove it
	for (row = numOfRows - 1; row >= 0; row--) {
		if (isRowFilled(row)) {
			removeRow(row);
			rowsRemoved++;
		}
	}
	return rowsRemoved;
}

void Grid::removeRow(int row) {
	int newRow, col, depth;
	//Set each block in the row to null
	if (row < numOfRows) {
		for (col = 0; col < numOfColumns; col++) {
			for (depth = 0; depth < depthSize; depth++) {
				delete grid[row][col][depth];
				grid[row][col][depth] = NULL;
			}
		}
	}

	//Move all blocks above the deleted row down a row
	for (newRow = row; newRow < numOfRows - 1; newRow++) {
		for (col = 0; col < numOfColumns; col++) {
			for (depth = 0; depth < depthSize; depth++) {
				grid[newRow][col][depth] = grid[newRow + 1][col][depth];
			}
		}
	}
}

void Grid::printGrid() {
	printf("\n");
	int row, col;
	//Print X if grid position has a block or O if it does not
	for (row = numOfRows - 1; row >= 0; row--) {
		for (col = 0; col < numOfColumns; col++) {
			if (isPositionFilled(row, col, 0)) {
				printf("X");
			} else {
				printf("0");
			}
		}
		printf("\n");
	}
}

int Grid::getNumOfRows() {
	return numOfRows;
}
int Grid::getNumOfColumns() {
	return numOfColumns;
}
int Grid::getDepthSize() {
	return depthSize;
}

void Grid::deleteAllBlocks() {
	int row, col, depth;

	//Delete each block in the grid
	for (row = 0; row < numOfRows; row++) {
		for (col = 0; col < numOfColumns; col++) {
			for (depth = 0; depth < depthSize; depth++) {
				delete grid[row][col][depth];
				grid[row][col][depth] = NULL;
			}
		}
	}
}

