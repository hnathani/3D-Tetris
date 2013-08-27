
#ifndef VIEW_H_
#define VIEW_H_

#include "Models/Game.h"
#include "lib/Angel.h"
#include "Shaders/Shader.h"
#include "View/ViewingGrid.h"
#include "GL/glui.h"
#include "Transformations/Sphere.h"

#define BLOCK_PIXEL_SIZE 20
#define CANVAS 500
#define P_ORTHO 0
#define P_PERS 1
#define SPHERE_POINTS 212
#define GOURAUD_SHADING 1
#define FLAT_SHADING 2

//Initializes Graphics and user interface
void initView(Game *game, int argc, char ** argv);


#endif /* VIEW_H_ */
