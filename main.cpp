#include "Models/Game.h"
#include "View.h"
#include <stdio.h>
#include <string.h>

#ifndef GL_VERSION_3_2
#error "OpenGL >= 3.2 required"
#endif // GL_VERSION_3_2

//Creates the tetris game and initializes the graphics
int main(int argc, char ** argv) {
	srand(time(NULL));
	Game tetris = Game();
	initView(&tetris,  argc, argv);
	return 0;
}
