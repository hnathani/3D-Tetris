#ifndef H_SHAPE
#define H_SHAPE

#define SHAPE_I 0
#define SHAPE_L 1
#define SHAPE_SQ 2
#define SHAPE_S 3
#define SHAPE_T 4
#define SHAPE_T_RIGHT 5
#define SHAPE_T_LEFT 6
#define SHAPE_TRIPOD 7

#include <vector>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "Tetromino.h"
#include "../Transformations/TetrominoTransforms.h"
class ShapeGen {

public:
	//Sets up the shape generator based on the grid size
	ShapeGen(int rowSize, int colSize, int depthSize);

	//Creates a random shape and returns a vector of blocks for that shape
	Tetromino* createShape();

	//Generates a random color
	void randomColor(vec4 &color);

private:
	float shapes[8][5][3];

	//Creates a shape definition matrix with the specified grid sizes
	void createMatrix(int rowSize, int colSize, int depthSize);
};

#endif
