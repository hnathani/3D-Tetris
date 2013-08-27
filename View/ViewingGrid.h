#ifndef H_VIEWING_GRID
#define H_VIEWING_GRID

#include "../Models/Game.h"

#define VG_FULL_WIRE_FRAME 0
#define VG_HALF_WIRE_FRAME 1
#define VG_MAX_GRID_SIZE GRID_COL * GRID_ROW * GRID_DEPTH
#define VG_NUMBER_OF_FACES 6



class ViewingGrid {
public:
	ViewingGrid();
	void getGridPoints(float rotation, vec4 *buffer, int &size, int gridType);
	void getGridColor(vec4 *buffer);
	int getSize();

	vec4 FullGrid[VG_MAX_GRID_SIZE];
	vec4 partialGrid[VG_MAX_GRID_SIZE];
	vec4 colorBuffer[VG_MAX_GRID_SIZE];
	void calculatePartialGrid(float rotationX, float rotationY);
	void toggleGridType();
	int getGridType();

private:

	void setDisplayedFaces(float rotationX, float rotationY);
	void calculatePartialGridFaces();
	void calculateFullGrid();
	void calculateColor();
	int fullGridSize;
	int partialGridSize;

	vec4 gridFaces[6][GRID_COL * GRID_ROW];
	int faceSizes[6];
	bool displayFaces[6];
	int gridType;
};

#endif
