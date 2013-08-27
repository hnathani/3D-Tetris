#ifndef TETROMINO_TRANSFORMS_H_
#define TETROMINO_TRANSFORMS_H_

#include "../Models/Tetromino.h"
#include "../Models/Grid.h"
#include "math.h"
#define CCW 90
#define CW -90
#define ROTATE_X 1
#define ROTATE_Y 2
#define ROTATE_Z 3

bool canAdd(Tetromino &tetro, Grid &grid);

//Moves the tetromino to the bottom of the grid
void toBottom(Tetromino &tetro, Grid &grid);

//Attempts to move the shape specified by the x and y values.
//The attempt will fail if translation moves the shape off the grid or if the spot is taken by another shape.
bool move(Tetromino &tetro, Grid &grid, int x, int y, int z);

//Checks if rotation is possible
bool canRotate(Tetromino &tetro, Grid &grid, int theta, int type);

//Checks if movement is possible
bool canMove(Tetromino &tetro, Grid &grid, int x, int y, int z);

//Attempts to rotate the tetromino in the direction specified type
//If rotation is valid returns true otherwise false
bool rotateTet(Tetromino &tetro, Grid &grid, int theta, int type);

//Moves the shape to the lowest available location
bool shapeAtBottom(Tetromino &tetro, Grid &grid);

//Calculate points for a tetromino block
void calculateTetrominoBlk(vec4 *points, vec4 *tempPoints, int i);

//Returns the rotation matrix for that direction and type;
mat4 getRotationM(float theta, int type, float originX, float originY, float originZ);

//Get the tetrominos distance from the bottom
int distanceFromBottom(Tetromino &tetro, Grid &grid);

#endif /* TRANSFORMATIONS_H_ */
