#include "Tetromino.h"

Tetromino::Tetromino(vec4 points[T_POINTS], vec4 color, vec4 pivotPoint) {
	int i;
	//Set points

	setPoints(points);

	//Set color and pivot point
	for (i = 0; i < 4; i++) {
		this->color[i] = color[i];
		this->pivotPoint[i] = pivotPoint[i];
	}
}

Tetromino::~Tetromino() {
}

void Tetromino::setPoints(vec4 points[T_POINTS]) {
	int i;
	for (i = 0; i < T_POINTS; i++) {
		//Set X Y Z H
		this->points[i][0] = points[i][0];
		this->points[i][1] = points[i][1];
		this->points[i][2] = points[i][2];
		this->points[i][3] = points[i][3];
	}
}
