#ifndef H_GAME
#define H_GAME

#include "../Transformations/TetrominoTransforms.h"
#include "Grid.h"
#include "ShapeGen.h"

#define GRID_ROW 20
#define GRID_COL 10
#define GRID_DEPTH 10
#define ANIMATE_STEPS 5

class Game {
public:
	Game();
	//Updates game status
	void logic();

	//Gets tetromino points
	bool getTetro(vec4 *points, vec4 *color);

	//Gets grid block points
	bool getBlocks(vec4 *points, vec4 *colors, int &size);

	//Gets Tetromino rotation and translation
	bool getTetroTrans(mat4 &transformation);

	//Moves tetromino in X direction
	void moveTetrominoX(int x);

	//Moves tetromino in Y direction
	void moveTetrominoY(int y);

	//Moves tetromino in Z direction
	void moveTetrominoZ(int z);

	//Rotate tetromino by a specified amount in degree and the rotation axis
	void rotateTetromino(int rotate, int rotationType);

	//Move tetromino to the bottom of the grid
	void moveTetrominoBottom();

	//Start a new game and reset all the values
	void reset();

	//Get the current score of the game
	int getScore();

	//Check if the game is running
	bool isGameOver();

	//Get the centers of the tetro blocks
	void getTetroCenters(vec4 *points);

	//Check if there is currently a tetromino in play
	bool isTetroFalling();

	//Get the center points of every block in grid
	void getGridCenters(vec4 *points, vec4 *colors, int &size);

	//Get the number of spaces until the tetromino can longer be moved to the bottom
	int getMovementToBottom();

	Tetromino *currTetro;
	bool animating;
private:
	void checkTransformation();
	void checkAnimation();
	void getBlocksPoints(vec4 *points, int offset, int row, int col, int depth);
	void getBlockColors(vec4 *colors, int offset, int row, int col, int depth);
	bool shapeInFlight, moveToBottom, gameStarted, newblock;
	float moveX, moveY, moveZ;
	float animateX, animateY, animateZ;
	int rotationType, rotate, score;

	//Animation globals
	int animationState, animationRotationType;
	float animationRotation;
};

#endif
