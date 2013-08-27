#include "ViewingGrid.h"

enum {
	VG_FACE_BACK,
	VG_FACE_FRONT,
	VG_FACE_LEFT,
	VG_FACE_RIGHT,
	VG_FACE_TOP,
	VG_FACE_BOTTOM
};

ViewingGrid::ViewingGrid() {
	calculateFullGrid();
	calculateColor();
	calculatePartialGridFaces();
	calculatePartialGrid(0, 0);
	gridType = VG_HALF_WIRE_FRAME;
}

void ViewingGrid::calculatePartialGrid(float rotationX, float rotationY) {
	setDisplayedFaces(rotationX, rotationY);
	int i, j, index = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < faceSizes[i]; j++) {
			if (displayFaces[i]) {
				partialGrid[index] = gridFaces[i][j];
				index++;
			}
		}
	}
	partialGridSize = index + 1;
}

int ViewingGrid::getSize() {
	if(gridType == VG_FULL_WIRE_FRAME) {
		return fullGridSize;
	}
	return partialGridSize;
}

void ViewingGrid::setDisplayedFaces(float rotationX, float rotationY) {
	if (rotationX < 0) {
		rotationX = rotationX + 360;
	}
	if (rotationY < 0) {
		rotationY = rotationY + 360;
	}
	//Check to show the back face
	bool xReq = (rotationX >= 0 && rotationX <= 90) || (rotationX >= 270
			&& rotationX <= 360);
	bool yReq = (rotationY >= 90 && rotationY <= 270);
	if (xReq && yReq || !xReq && !yReq) {
		displayFaces[0] = false;
	} else {
		displayFaces[0] = true;
	}

	//Check to show the front face
	xReq = (rotationX >= 0 && rotationX <= 90) || (rotationX >= 270
			&& rotationX <= 360);
	yReq = (rotationY >= 0 && rotationY <= 90) || (rotationY >= 270
			&& rotationY <= 360);
	if (xReq && yReq || !xReq && !yReq) {
		displayFaces[1] = false;
	} else {
		displayFaces[1] = true;
	}

	//Check to show the left face
	yReq = (rotationY >= 0 && rotationY <= 180);
	if (xReq && yReq || !xReq && yReq) {
		displayFaces[2] = false;
	} else {
		displayFaces[2] = true;
	}

	//Check to show the right face
	yReq = (rotationY >= 180 && rotationY <= 360);
	if (xReq && yReq || !xReq && yReq) {
		displayFaces[3] = false;
	} else {
		displayFaces[3] = true;
	}

	//Check to show the top face
	xReq = (rotationX >= 180 && rotationX <= 360);
	yReq = (rotationY >= 90 && rotationY <= 270);
	if (xReq && yReq || !xReq && !yReq) {
		displayFaces[4] = false;
	} else {
		displayFaces[4] = true;
	}

	//Check to show the bottom face
	xReq = (rotationX >= 0 && rotationX <= 180);
	if (xReq && yReq || !xReq && !yReq) {
		displayFaces[5] = false;
	} else {
		displayFaces[5] = true;
	}
}

void ViewingGrid::calculatePartialGridFaces() {
	int index = 0, depth, row, col;

	//Face 1 Back
	for (row = 0; row <= GRID_ROW; row++) {
		gridFaces[0][index] = vec4(0, row, 0, 1);
		gridFaces[0][index + 1] = vec4(GRID_COL, row, 0, 1);
		index += 2;
	}
	for (col = 0; col <= GRID_COL; col++) {
		gridFaces[0][index] = vec4(col, 0, 0, 1);
		gridFaces[0][index + 1] = vec4(col, GRID_ROW, 0, 1);
		index += 2;
	}
	faceSizes[0] = index;
	index = 0;
	//Face 2 Front
	for (row = 0; row <= GRID_ROW; row++) {
		gridFaces[1][index] = vec4(0, row, GRID_DEPTH, 1);
		gridFaces[1][index + 1] = vec4(GRID_COL, row, GRID_DEPTH, 1);
		index += 2;
	}
	for (col = 0; col <= GRID_COL; col++) {
		gridFaces[1][index] = vec4(col, 0, GRID_DEPTH, 1);
		gridFaces[1][index + 1] = vec4(col, GRID_ROW, GRID_DEPTH, 1);
		index += 2;
	}
	faceSizes[1] = index;
	index = 0;

	//Face 3 Left
	for (row = 0; row <= GRID_ROW; row++) {
		gridFaces[2][index] = vec4(0, row, 0, 1);
		gridFaces[2][index + 1] = vec4(0, row, GRID_DEPTH, 1);
		index += 2;
	}
	for (depth = 0; depth <= GRID_DEPTH; depth++) {
		gridFaces[2][index] = vec4(0, 0, depth, 1);
		gridFaces[2][index + 1] = vec4(0, GRID_ROW, depth, 1);
		index += 2;
	}
	faceSizes[2] = index;
	index = 0;

	//Face 4 Right
	for (row = 0; row <= GRID_ROW; row++) {
		gridFaces[3][index] = vec4(GRID_COL, row, 0, 1);
		gridFaces[3][index + 1] = vec4(GRID_COL, row, GRID_DEPTH, 1);
		index += 2;
	}
	for (depth = 0; depth <= GRID_DEPTH; depth++) {
		gridFaces[3][index] = vec4(GRID_COL, 0, depth, 1);
		gridFaces[3][index + 1] = vec4(GRID_COL, GRID_ROW, depth, 1);
		index += 2;
	}
	faceSizes[3] = index;
	index = 0;

	//Face 5 Top
	for (col = 0; col <= GRID_COL; col++) {
		gridFaces[4][index] = vec4(col, GRID_ROW, 0, 1);
		gridFaces[4][index + 1] = vec4(col, GRID_ROW, GRID_DEPTH, 1);
		index += 2;
	}
	for (depth = 0; depth <= GRID_DEPTH; depth++) {
		gridFaces[4][index] = vec4(0, GRID_ROW, depth, 1);
		gridFaces[4][index + 1] = vec4(GRID_COL, GRID_ROW, depth, 1);
		index += 2;
	}

	faceSizes[4] = index;
	index = 0;

	//Face 6 Bottom
	for (col = 0; col <= GRID_COL; col++) {
		gridFaces[5][index] = vec4(col, 0, 0, 1);
		gridFaces[5][index + 1] = vec4(col, 0, GRID_DEPTH, 1);
		index += 2;
	}
	for (depth = 0; depth <= GRID_DEPTH; depth++) {
		gridFaces[5][index] = vec4(0, 0, depth, 1);
		gridFaces[5][index + 1] = vec4(GRID_COL, 0, depth, 1);
		index += 2;
	}
	faceSizes[5] = index;
}

void ViewingGrid::calculateFullGrid() {
	int row, col, depth, index = 0;
	for (depth = 0; depth <= GRID_DEPTH; depth++) {
		for (row = 0; row <= GRID_ROW; row++) {
			FullGrid[index] = vec4(0, row, depth, 1);
			FullGrid[index + 1] = vec4(GRID_COL, row, depth, 1);
			index += 2;
		}
		for (col = 0; col <= GRID_COL; col++) {
			FullGrid[index] = vec4(col, 0, depth, 1);
			FullGrid[index + 1] = vec4(col, GRID_ROW, depth, 1);
			index += 2;
		}
	}

	for (row = 0; row <= GRID_ROW; row++) {
		for (col = 0; col <= GRID_COL; col++) {
			FullGrid[index] = vec4(col, row, 0, 1);
			FullGrid[index + 1] = vec4(col, row, GRID_DEPTH, 1);
			index += 2;
		}
	}
	fullGridSize = index + 1;
}

void ViewingGrid::calculateColor() {
	int i;
	for (i = 0; i < VG_MAX_GRID_SIZE; i++) {
		colorBuffer[i] = vec4(1.0, 1.0, 1.0, 1);
	}
}

void ViewingGrid::toggleGridType() {
	if(gridType == VG_FULL_WIRE_FRAME) {
		gridType = VG_HALF_WIRE_FRAME;
	} else {
		gridType = VG_FULL_WIRE_FRAME;
	}
}


int ViewingGrid::getGridType() {
	return gridType;
}

