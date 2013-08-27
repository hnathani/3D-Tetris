/*
 * Transformations.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: hussein
 */

#include "TetrominoTransforms.h"

bool shapeAtBottom(Tetromino &tetro, Grid &grid) {
	if (move(tetro, grid, 0, -1, 0)) {
		move(tetro, grid, 0, 1, 0);
		return false;
	}
	return true;
}

bool move(Tetromino &tetro, Grid &grid, int x, int y, int z) {
	int i, row, col;
	//Assign new points
	for (i = 0; i < T_POINTS; i++) {
		tetro.points[i][0] += x;
		tetro.points[i][1] += y;
		tetro.points[i][2] += z;
	}

	if (grid.canAddBlocks(tetro.points, T_POINTS)) {
		//Update pivot point
		tetro.pivotPoint[0] += x;
		tetro.pivotPoint[1] += y;
		tetro.pivotPoint[2] += z;
	} else {
		//Assign old points
		for (i = 0; i < T_POINTS; i++) {
			tetro.points[i][0] -= x;
			tetro.points[i][1] -= y;
			tetro.points[i][2] -= z;
		}
		return false;
	}

	return true;
}

bool canMove(Tetromino &tetro, Grid &grid, int x, int y, int z) {
	int i, row, col;
	bool success = false;

	if(x == 0 && y == 0 && z == 0) {
		return false;
	}

	//Assign new points
	for (i = 0; i < T_POINTS; i++) {
		tetro.points[i][0] += x;
		tetro.points[i][1] += y;
		tetro.points[i][2] += z;
	}

	if (grid.canAddBlocks(tetro.points, T_POINTS)) {
		success = true;
	}
	//Assign old points
	for (i = 0; i < T_POINTS; i++) {
		tetro.points[i][0] -= x;
		tetro.points[i][1] -= y;
		tetro.points[i][2] -= z;
	}
	return success;
}

int distanceFromBottom(Tetromino &tetro, Grid &grid){
	int i = -1;
	while (canMove(tetro, grid, 0, i, 0)) {
		i--;
	}
	return i;
}

void toBottom(Tetromino &tetro, Grid &grid) {
	//Continuously move the shape down by 1 row until movement fails
	while (move(tetro, grid, 0, -1, 0))
		;
}

void copyPoints(vec4 from[], vec4 to[], int size) {
	int i, j;
	for (i = 0; i < size; i++) {
		to[i][0] = from[i][0];
		to[i][1] = from[i][1];
		to[i][2] = from[i][2];
		to[i][3] = from[i][3];
	}
}

void calculateTetrominoBlk(vec4 *points, vec4 *tempPoints, int i) {

	points[i] = tempPoints[0];
	points[i + 1] = tempPoints[1];
	points[i + 2] = tempPoints[2];
	points[i + 3] = tempPoints[2];
	points[i + 4] = tempPoints[3];
	points[i + 5] = tempPoints[1];

	points[i + 6] = tempPoints[6];
	points[i + 7] = tempPoints[2];
	points[i + 8] = tempPoints[3];
	points[i + 9] = tempPoints[3];
	points[i + 10] = tempPoints[7];
	points[i + 11] = tempPoints[6];

	points[i + 12] = tempPoints[3];
	points[i + 13] = tempPoints[7];
	points[i + 14] = tempPoints[1];
	points[i + 15] = tempPoints[1];
	points[i + 16] = tempPoints[5];
	points[i + 17] = tempPoints[7];

	points[i + 18] = tempPoints[1];
	points[i + 19] = tempPoints[5];
	points[i + 20] = tempPoints[4];
	points[i + 21] = tempPoints[4];
	points[i + 22] = tempPoints[0];
	points[i + 23] = tempPoints[1];

	points[i + 24] = tempPoints[0];
	points[i + 25] = tempPoints[4];
	points[i + 26] = tempPoints[2];
	points[i + 27] = tempPoints[2];
	points[i + 28] = tempPoints[4];
	points[i + 29] = tempPoints[6];

	points[i + 30] = tempPoints[4];
	points[i + 31] = tempPoints[5];
	points[i + 32] = tempPoints[6];
	points[i + 33] = tempPoints[6];
	points[i + 34] = tempPoints[7];
	points[i + 35] = tempPoints[5];

}

bool rotateTet(Tetromino &tetro, Grid &grid, int theta, int type) {
	float originX = tetro.pivotPoint[0];
	float originY = tetro.pivotPoint[1];
	float originZ = tetro.pivotPoint[2];
	vec4 tempCoords[T_POINTS];
	mat4 rot = getRotationM(theta, type, originX, originY, originZ);
	int i;
	for (i = 0; i < T_POINTS; i++) {
		tempCoords[i] = rot * tetro.points[i];
	}

	if (grid.canAddBlocks(tempCoords, T_POINTS)) {
		copyPoints(tempCoords, tetro.points, T_POINTS);
	} else {

		return false;
	}

	return true;
}

bool canRotate(Tetromino &tetro, Grid &grid, int theta, int type) {
	float originX = tetro.pivotPoint[0];
	float originY = tetro.pivotPoint[1];
	float originZ = tetro.pivotPoint[2];
	vec4 tempCoords[T_POINTS];
	mat4 rot = getRotationM(theta, type, originX, originY, originZ);

	int i;
	for (i = 0; i < T_POINTS; i++) {
		tempCoords[i] = rot * tetro.points[i];
	}

	if (!grid.canAddBlocks(tempCoords, T_POINTS)) {
		return false;
	}

	return true;
}

mat4 getRotationM(float theta, int type, float originX, float originY,
		float originZ) {
	mat4 rot;

	if (type == ROTATE_X) {
		rot = RotateX(theta);
	} else if (type == ROTATE_Y) {
		rot = RotateY(theta);
	} else if (type == ROTATE_Z) {
		rot = RotateZ(theta);
	}
	mat4 toOrigin = Translate(-originX, -originY, -originZ);
	mat4 fromOrigin = Translate(originX, originY, originZ);

	rot = fromOrigin * rot * toOrigin;
	return rot;
}

