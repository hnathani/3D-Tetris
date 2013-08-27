#ifndef H_TETROMINO
#define H_TETROMINO

#include "../lib/Angel.h"

#define T_POINTS 144
#define T_POINTS_PER_BLOCK 36
#define T_BLOCKS 4
class Tetromino {
public:
	//Creates a tetromino with the specified points, color, and pivot point
	Tetromino (vec4 points[T_POINTS], vec4 color, vec4 pivotPoint);
	virtual ~Tetromino();
	void setPoints(vec4 points[T_POINTS]);

	vec4 points[T_POINTS];
	vec4 color;
	vec4 pivotPoint;
};



#endif
