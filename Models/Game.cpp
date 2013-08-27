/*
 * GameController.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: hussein
 */

#include "Game.h"

Grid grid = Grid(GRID_ROW, GRID_COL, GRID_DEPTH);
ShapeGen sgen = ShapeGen(GRID_ROW, GRID_COL, GRID_DEPTH);

Game::Game() {
	shapeInFlight = false;
	moveToBottom = false;
	gameStarted = true;
	newblock = false;
	moveX = 0.0;
	moveY = 0.0;
	moveZ = 0.0;
	rotate = 0.0;
	animationState = 0;
	animationRotation = 0;
	animationRotationType = 0;
	animating = false;
	score = 0;
}

void Game::logic() {
	if (gameStarted) {
		if (shapeInFlight) {
			//Check if there is any transformations to perform on tetromino
			checkTransformation();
		} else if (!shapeInFlight) {
			//Create a tetromino and check if it can be added to the grid
			currTetro = sgen.createShape();
			if (grid.canAddBlocks(currTetro->points, T_POINTS)) {
				shapeInFlight = true;
			} else {
				gameStarted = false;
			}
		}
	}
}

void Game::checkTransformation() {
	if (!animating) {

		//Move block to bottom if command specified
		if (moveToBottom) {
			toBottom(*currTetro, grid);
			moveToBottom = false;
		}
		bool movement, rotation;
		movement = canMove(*currTetro, grid, moveX, moveY, moveZ);
		rotation = canRotate(*currTetro, grid, rotate, rotationType);

		//Check if there are any rotations / translations to perform
		if ((rotate != 0) || movement) {
			animating = true;
			animationState = 1;

			if (rotation) {
				animationRotation = rotate;
				animationRotationType = rotationType;
			}

			animateX = moveX;
			animateY = moveY;
			animateZ = moveZ;

		}
		rotate = 0;
		moveX = 0;
		moveY = 0;
		moveZ = 0;
		if (shapeAtBottom(*currTetro, grid)) {
			shapeInFlight = false;
			grid.addBlocks(currTetro->points, currTetro->color, T_POINTS);
			score += grid.checkForFullRows();
			delete currTetro;
		}
	}
}

bool Game::getTetro(vec4 *points, vec4 *color) {
	//Only return points if there is a tetromino in the grid
	if (shapeInFlight) {
		int size = 0;
		int i;
		for (i = 0; i < (T_POINTS); i++) {
			points[i][0] = currTetro->points[i][0];
			points[i][1] = currTetro->points[i][1];
			points[i][2] = currTetro->points[i][2];
			points[i][3] = currTetro->points[i][3];
			color[i][0] = currTetro->color[0];
			color[i][1] = currTetro->color[1];
			color[i][2] = currTetro->color[2];
			color[i][3] = currTetro->color[3];
			size += 4;
		}
		return true;
	} else {
		return false;
	}
}
bool Game::getTetroTrans(mat4 &transformation) {
	if (animating) {
		//Check if animation is over and reset rotation and movement parameters
		if (animationState == ANIMATE_STEPS) {
			animating = false;
			animationState = 0;
			if (floor(animationRotation + .5) != 0) {
				int r = animationRotation;
				rotateTet(*currTetro, grid, animationRotation,
						animationRotationType);
			}
			move(*currTetro, grid, animateX, animateY, animateZ);

			animateX = 0;
			animateY = 0;
			animateZ = 0;
			animationRotation = 0;
			animationRotationType = 0;
		}

		//Calculate transformation matrix
		float st = animationState;
		float step = ANIMATE_STEPS;
		float diff = st / step;
		vec4 p1 = currTetro->pivotPoint + vec4(animateX * diff,
				animateY * diff, animateZ * diff, 0);

		if (animationRotationType == ROTATE_X) {
			transformation = Translate(p1) * RotateX(animationRotation * diff)
					* Translate(-p1);
		} else if (animationRotationType == ROTATE_Y) {
			transformation = Translate(p1) * RotateY(animationRotation * diff)
					* Translate(-p1);
		} else {
			transformation = Translate(p1) * RotateZ(animationRotation * diff)
					* Translate(-p1);
		}
		transformation = transformation * Translate(vec4(animateX * diff,
				animateY * diff, animateZ * diff, 1));
		animationState++;

	} else {
		return false;
	}
	return true;
}
bool Game::getBlocks(vec4 *points, vec4 *colors, int &size) {
	//Return the points and colors of blocks in the grid
	size = 0;
	int row, col, depth;
	for (row = 0; row < GRID_ROW; row++) {
		for (col = 0; col < GRID_COL; col++) {
			for (depth = 0; depth < GRID_DEPTH; depth++) {
				if (grid.isPositionFilled(row, col, depth)) {
					getBlocksPoints(points, size, row, col, depth);
					getBlockColors(colors, size, row, col, depth);
					size += T_POINTS_PER_BLOCK;
				}
			}
		}
	}
}

void Game::getBlocksPoints(vec4 *points, int offset, int row, int col,
		int depth) {
	//Calculate block points based on grid positions
	vec4 tempPoints[8];
	tempPoints[0] = vec4(col, row, depth, 1.0f);
	tempPoints[1] = vec4(col, row, depth + 1, 1.0f);
	tempPoints[2] = vec4(col + 1, row, depth, 1.0f);
	tempPoints[3] = vec4(col + 1, row, depth + 1, 1.0f);
	tempPoints[4] = vec4(col, row + 1, depth, 1.0f);
	tempPoints[5] = vec4(col, row + 1, depth + 1, 1.0f);
	tempPoints[6] = vec4(col + 1, row + 1, depth, 1.0f);
	tempPoints[7] = vec4(col + 1, row + 1, depth + 1, 1.0f);
	calculateTetrominoBlk(points, tempPoints, offset);
}
void Game::getBlockColors(vec4 *colors, int offset, int row, int col, int depth) {
	//Set block vertices to the color
	int i;
	for (i = 0; i < T_POINTS_PER_BLOCK + 2; i++) {
		colors[offset + i][0] = grid.grid[row][col][depth]->color[0];
		colors[offset + i][1] = grid.grid[row][col][depth]->color[1];
		colors[offset + i][2] = grid.grid[row][col][depth]->color[2];
		colors[offset + i][3] = grid.grid[row][col][depth]->color[3];
	}

}
void Game::moveTetrominoX(int x) {
	moveX = x;
}
void Game::moveTetrominoY(int y) {
	moveY = y;
}
void Game::moveTetrominoZ(int z) {
	moveZ = z;
}
void Game::rotateTetromino(int rotate, int rotationType) {
	this->rotate = rotate;
	this->rotationType = rotationType;
}
void Game::moveTetrominoBottom() {
	moveToBottom = true;
}

void Game::reset() {
	moveToBottom = false;
	gameStarted = true;
	newblock = false;
	moveX = 0.0;
	moveY = 0.0;
	moveZ = 0.0;
	rotate = 0.0;
	animationState = 0;
	animationRotation = 0;
	animationRotationType = 0;
	animating = false;
	score = 0;

	grid.deleteAllBlocks();
	if (shapeInFlight) {
		delete currTetro;
		shapeInFlight = false;
	}
}

int Game::getScore() {
	return score;
}
bool Game::isGameOver() {
	return !gameStarted;
}

void Game::getTetroCenters(vec4 *points) {
	//Get the center of each tetromino block
	vec4 p1 = vec4(.5, .5, .5, 0);
	points[0] = currTetro->points[0] + p1;
	points[1] = currTetro->points[T_POINTS_PER_BLOCK] + p1;
	points[2] = currTetro->points[T_POINTS_PER_BLOCK * 2] + p1;
	points[3] = currTetro->points[T_POINTS_PER_BLOCK * 3] + p1;
}

void Game::getGridCenters(vec4 *points, vec4 *colors, int &size) {
	size = 0;
	int row, col, depth;
	for (row = 0; row < GRID_ROW; row++) {
		for (col = 0; col < GRID_COL; col++) {
			for (depth = 0; depth < GRID_DEPTH; depth++) {
				if (grid.isPositionFilled(row, col, depth)) {
					points[size] = vec4(col + .5, row + .5, depth + .5, 1);
					colors[size] = grid.grid[row][col][depth]->color;
					size++;
				}
			}
		}
	}
}

bool Game::isTetroFalling() {
	return shapeInFlight;
}

int Game::getMovementToBottom() {
	return distanceFromBottom(*currTetro, grid);
}
