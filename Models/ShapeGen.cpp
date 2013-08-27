#include "ShapeGen.h"

ShapeGen::ShapeGen(int rowSize, int colSize, int depthSize) {
	createMatrix(rowSize, colSize, depthSize);
}

void ShapeGen::createMatrix(int rowSize, int colSize, int depthSize) {

	int colCenter = colSize / 2 - 1;
	int depthCenter = depthSize / 2 - 1;
	rowSize = rowSize - 1;

	//Shape Definitions based on row and column size of the grid
	//Straight shape

	shapes[SHAPE_I][0][0] = colCenter;
	shapes[SHAPE_I][1][0] = colCenter;
	shapes[SHAPE_I][2][0] = colCenter;
	shapes[SHAPE_I][3][0] = colCenter;
	shapes[SHAPE_I][4][0] = colCenter + .5;

	shapes[SHAPE_I][0][1] = rowSize;
	shapes[SHAPE_I][1][1] = rowSize;
	shapes[SHAPE_I][2][1] = rowSize;
	shapes[SHAPE_I][3][1] = rowSize;
	shapes[SHAPE_I][4][1] = rowSize + .5;

	shapes[SHAPE_I][0][2] = depthCenter - 1;
	shapes[SHAPE_I][1][2] = depthCenter;
	shapes[SHAPE_I][2][2] = depthCenter + 1;
	shapes[SHAPE_I][3][2] = depthCenter + 2;
	shapes[SHAPE_I][4][2] = depthCenter + .5;

	//L
	shapes[SHAPE_L][0][0] = colCenter;
	shapes[SHAPE_L][1][0] = colCenter;
	shapes[SHAPE_L][2][0] = colCenter;
	shapes[SHAPE_L][3][0] = colCenter - 1;
	shapes[SHAPE_L][4][0] = colCenter + .5;

	shapes[SHAPE_L][0][1] = rowSize;
	shapes[SHAPE_L][1][1] = rowSize;
	shapes[SHAPE_L][2][1] = rowSize;
	shapes[SHAPE_L][3][1] = rowSize;
	shapes[SHAPE_L][4][1] = rowSize + .5;

	shapes[SHAPE_L][0][2] = depthCenter - 1;
	shapes[SHAPE_L][1][2] = depthCenter;
	shapes[SHAPE_L][2][2] = depthCenter + 1;
	shapes[SHAPE_L][3][2] = depthCenter + 1;
	shapes[SHAPE_L][4][2] = depthCenter + .5;

	//Square
	shapes[SHAPE_SQ][0][0] = colCenter;
	shapes[SHAPE_SQ][1][0] = colCenter;
	shapes[SHAPE_SQ][2][0] = colCenter + 1;
	shapes[SHAPE_SQ][3][0] = colCenter + 1;
	shapes[SHAPE_SQ][4][0] = colCenter + 1;

	shapes[SHAPE_SQ][0][1] = rowSize;
	shapes[SHAPE_SQ][1][1] = rowSize;
	shapes[SHAPE_SQ][2][1] = rowSize;
	shapes[SHAPE_SQ][3][1] = rowSize;
	shapes[SHAPE_SQ][4][1] = rowSize + 1;

	shapes[SHAPE_SQ][0][2] = depthCenter;
	shapes[SHAPE_SQ][1][2] = depthCenter - 1;
	shapes[SHAPE_SQ][2][2] = depthCenter - 1;
	shapes[SHAPE_SQ][3][2] = depthCenter;
	shapes[SHAPE_SQ][4][2] = depthCenter;

	//T shape
	shapes[SHAPE_T][0][0] = colCenter;
	shapes[SHAPE_T][1][0] = colCenter;
	shapes[SHAPE_T][2][0] = colCenter + 1;
	shapes[SHAPE_T][3][0] = colCenter - 1;
	shapes[SHAPE_T][4][0] = colCenter + .5;

	shapes[SHAPE_T][0][1] = rowSize;
	shapes[SHAPE_T][1][1] = rowSize - 1;
	shapes[SHAPE_T][2][1] = rowSize - 1;
	shapes[SHAPE_T][3][1] = rowSize - 1;
	shapes[SHAPE_T][4][1] = rowSize - .5;

	shapes[SHAPE_T][0][2] = depthCenter;
	shapes[SHAPE_T][1][2] = depthCenter;
	shapes[SHAPE_T][2][2] = depthCenter;
	shapes[SHAPE_T][3][2] = depthCenter;
	shapes[SHAPE_T][4][2] = depthCenter + .5;

	//S shape
	shapes[SHAPE_S][0][0] = colCenter + 1;
	shapes[SHAPE_S][1][0] = colCenter;
	shapes[SHAPE_S][2][0] = colCenter;
	shapes[SHAPE_S][3][0] = colCenter - 1;
	shapes[SHAPE_S][4][0] = colCenter + .5;

	shapes[SHAPE_S][0][1] = rowSize;
	shapes[SHAPE_S][1][1] = rowSize;
	shapes[SHAPE_S][2][1] = rowSize - 1;
	shapes[SHAPE_S][3][1] = rowSize - 1;
	shapes[SHAPE_S][4][1] = rowSize + .5;

	shapes[SHAPE_S][0][2] = depthCenter;
	shapes[SHAPE_S][1][2] = depthCenter;
	shapes[SHAPE_S][2][2] = depthCenter;
	shapes[SHAPE_S][3][2] = depthCenter;
	shapes[SHAPE_S][4][2] = depthCenter + .5;

	//Tower Right shape
	shapes[SHAPE_T_RIGHT][0][0] = colCenter;
	shapes[SHAPE_T_RIGHT][1][0] = colCenter;
	shapes[SHAPE_T_RIGHT][2][0] = colCenter;
	shapes[SHAPE_T_RIGHT][3][0] = colCenter - 1;
	shapes[SHAPE_T_RIGHT][4][0] = colCenter + .5;

	shapes[SHAPE_T_RIGHT][0][1] = rowSize;
	shapes[SHAPE_T_RIGHT][1][1] = rowSize - 1;
	shapes[SHAPE_T_RIGHT][2][1] = rowSize - 1;
	shapes[SHAPE_T_RIGHT][3][1] = rowSize - 1;
	shapes[SHAPE_T_RIGHT][4][1] = rowSize - .5;

	shapes[SHAPE_T_RIGHT][0][2] = depthCenter;
	shapes[SHAPE_T_RIGHT][1][2] = depthCenter;
	shapes[SHAPE_T_RIGHT][2][2] = depthCenter - 1;
	shapes[SHAPE_T_RIGHT][3][2] = depthCenter - 1;
	shapes[SHAPE_T_RIGHT][4][2] = depthCenter + .5;

	//Tower Left shape
	shapes[SHAPE_T_LEFT][0][0] = colCenter;
	shapes[SHAPE_T_LEFT][1][0] = colCenter;
	shapes[SHAPE_T_LEFT][2][0] = colCenter;
	shapes[SHAPE_T_LEFT][3][0] = colCenter + 1;
	shapes[SHAPE_T_LEFT][4][0] = colCenter + .5;

	shapes[SHAPE_T_LEFT][0][1] = rowSize;
	shapes[SHAPE_T_LEFT][1][1] = rowSize - 1;
	shapes[SHAPE_T_LEFT][2][1] = rowSize - 1;
	shapes[SHAPE_T_LEFT][3][1] = rowSize - 1;
	shapes[SHAPE_T_LEFT][4][1] = rowSize - .5;

	shapes[SHAPE_T_LEFT][0][2] = depthCenter;
	shapes[SHAPE_T_LEFT][1][2] = depthCenter;
	shapes[SHAPE_T_LEFT][2][2] = depthCenter - 1;
	shapes[SHAPE_T_LEFT][3][2] = depthCenter - 1;
	shapes[SHAPE_T_LEFT][4][2] = depthCenter + .5;

	//Tripod shape
	shapes[SHAPE_TRIPOD][0][0] = colCenter - 1;
	shapes[SHAPE_TRIPOD][1][0] = colCenter;
	shapes[SHAPE_TRIPOD][2][0] = colCenter;
	shapes[SHAPE_TRIPOD][3][0] = colCenter;
	shapes[SHAPE_TRIPOD][4][0] = colCenter + .5;

	shapes[SHAPE_TRIPOD][0][1] = rowSize - 1;
	shapes[SHAPE_TRIPOD][1][1] = rowSize - 1;
	shapes[SHAPE_TRIPOD][2][1] = rowSize;
	shapes[SHAPE_TRIPOD][3][1] = rowSize - 1;
	shapes[SHAPE_TRIPOD][4][1] = rowSize - .5;

	shapes[SHAPE_TRIPOD][0][2] = depthCenter;
	shapes[SHAPE_TRIPOD][1][2] = depthCenter;
	shapes[SHAPE_TRIPOD][2][2] = depthCenter;
	shapes[SHAPE_TRIPOD][3][2] = depthCenter - 1;
	shapes[SHAPE_TRIPOD][4][2] = depthCenter + .5;

}

Tetromino* ShapeGen::createShape() {
	//Generate random number to pick a shape;
	int shapeType = rand() % 8;
	vec4 color, points[T_POINTS], pivotPoint, tempPoints[8];
	randomColor(color);

	int i, j, blockNumber = 0;
	//Create each block of the tetromino
	for (i = 0; i < T_POINTS; i = i + T_POINTS_PER_BLOCK) {
		for (j = 0; j < 2; j++) {
			tempPoints[j * 4][0] = shapes[shapeType][blockNumber][0];
			tempPoints[j * 4][1] = shapes[shapeType][blockNumber][1] + j;
			tempPoints[j * 4][2] = shapes[shapeType][blockNumber][2];
			tempPoints[j * 4][3] = 1.0f;

			tempPoints[1 + j * 4][0] = shapes[shapeType][blockNumber][0];
			tempPoints[1 + j * 4][1] = shapes[shapeType][blockNumber][1] + j;
			tempPoints[1 + j * 4][2] = shapes[shapeType][blockNumber][2] + 1;
			tempPoints[1 + j * 4][3] = 1.0f;

			tempPoints[2 + j * 4][0] = shapes[shapeType][blockNumber][0] + 1;
			tempPoints[2 + j * 4][1] = shapes[shapeType][blockNumber][1] + j;
			tempPoints[2 + j * 4][2] = shapes[shapeType][blockNumber][2];
			tempPoints[2 + j * 4][3] = 1.0f;

			tempPoints[3 + j * 4][0] = shapes[shapeType][blockNumber][0] + 1;
			tempPoints[3 + j * 4][1] = shapes[shapeType][blockNumber][1] + j;
			tempPoints[3 + j * 4][2] = shapes[shapeType][blockNumber][2] + 1;
			tempPoints[3 + j * 4][3] = 1.0f;
		}
		calculateTetrominoBlk(points, tempPoints, i);
		blockNumber++;
	}

	pivotPoint[0] = shapes[shapeType][4][0];
	pivotPoint[1] = shapes[shapeType][4][1];
	pivotPoint[2] = shapes[shapeType][4][2];
	pivotPoint[3] = 1.0f;
	Tetromino *tetro = new Tetromino(points, color, pivotPoint);
	return tetro;

}

void ShapeGen::randomColor(vec4 &color) {
	//Randomly generate a red green and blue color
	color[0] = ((float) (rand() % 10 + 1)) / 10;
	color[1] = ((float) (rand() % 10 + 1)) / 10;
	color[2] = ((float) (rand() % 10 + 1)) / 10;
	color[3] = 1;
}

