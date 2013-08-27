/*
 * View.cpp
 *
 *  Created on: Oct 9, 2011
 *      Author: hussein
 */

#include "View.h"

Game *tetris;

GLuint gridBuffer[2];
GLuint blockBuffer[3];
GLuint tetrominoBuffer[3];
GLuint vertexArrays[3];

vec4 tetroPoints[SPHERE_POINTS * T_BLOCKS];
vec4 tetroColor[SPHERE_POINTS * T_BLOCKS];
vec4 tetroNormals[SPHERE_POINTS * T_BLOCKS];

vec4 blockPoints[GRID_COL * GRID_ROW * GRID_DEPTH * SPHERE_POINTS];
vec4 blockColors[GRID_COL * GRID_ROW * GRID_DEPTH * SPHERE_POINTS];
vec4 blockNormals[GRID_COL * GRID_ROW * GRID_DEPTH * SPHERE_POINTS];

GLuint modelView, projection, normalMatrix, useLighting, ambient, diffuse,
		specular;
float rotationX = -90, rotationY = 0, translationX = 0, translationY = 0,
		scale = .08;
int projType = P_ORTHO;
ViewingGrid vg = ViewingGrid();
bool drawSphere = false;
int shadingType = FLAT_SHADING;

//GLUI Globals
int window;
vec4 ambientComponent, diffuseComponent, specularComponent;
vec4 material_ambient, material_diffuse, material_specular;
int showLighting = 1;
GLUI_EditText *score, *status;

//Computes the normal of the input points
void normalize(vec4 *normals, vec4 *points, int numOfPoints) {
	int i, j, normalIndex = 0;
	int numOfFaces = numOfPoints / 6;
	int numOfBlocks = numOfPoints / 6;
	if (shadingType == FLAT_SHADING) {
		for (i = 0; i < numOfFaces; i++) {
			vec4 a = points[normalIndex + 1] - points[normalIndex + 0];
			vec4 b = points[normalIndex + 2] - points[normalIndex + 1];
			vec3 normal = cross(a, b);
			vec4 vNormal = vec4(normal[0], normal[1], normal[2], 1);
			normals[normalIndex] = vNormal;
			normals[normalIndex + 1] = vNormal;
			normals[normalIndex + 2] = vNormal;
			normals[normalIndex + 3] = vNormal;
			normals[normalIndex + 4] = vNormal;
			normals[normalIndex + 5] = vNormal;
			normalIndex += 6;
		}
	} else if (shadingType == GOURAUD_SHADING) {
		for (i = 0; i < numOfBlocks; i++) {
			vec3 totalNormal = vec3(0, 0, 0);
			for (j = 0; j < T_POINTS_PER_BLOCK; j = j + 6) {
				vec4 a = points[normalIndex + j + 1] - points[normalIndex + j];
				vec4 b = points[normalIndex + j + 2] - points[normalIndex + j
						+ 1];
				totalNormal = totalNormal + normalize(cross(a, b));
			}
			totalNormal = totalNormal / 6.0f;
			vec4 average = vec4(totalNormal[0], totalNormal[1], totalNormal[2],
					1);
			for (j = 0; j < T_POINTS_PER_BLOCK; j++) {
				normals[normalIndex + j] = average;
			}
			normalIndex += T_POINTS_PER_BLOCK;
		}

	}
}
//Compute transformation matrix
mat4 calcTransformationMatrix() {
	mat4 mv = Translate(-5, -10, -5);
	mv = RotateY(rotationY) * RotateX(rotationX) * Translate(translationX,
			translationY, 0) * Scale(scale, scale, scale) * mv;
	return mv;
}

void tetrominoTranslation(mat4 trans, vec4 *points, int size, vec4 *colors) {
	int i;
	for (i = 0; i < size; i++) {
		points[i] = trans * points[i];
		colors[i][3] = .4;
	}
}

//Initializes OpenGL buffers
void initBuffers(GLuint program, GLuint pos, GLuint color, GLuint normal) {
	glBindVertexArray(vertexArrays[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gridBuffer[0]);
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, gridBuffer[1]);
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindVertexArray(vertexArrays[1]);
	glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[0]);
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[1]);
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[2]);
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glBindVertexArray(vertexArrays[2]);
	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[0]);
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[1]);
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[2]);
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

}

//Initializes lighting parameters
void initLighting(GLuint program) {
	vec4 lightDirection(1, 1, 1, 1.0);
	ambientComponent = vec4(0.3, 0.5, 0.2, 1.0);
	diffuseComponent = vec4(1.0, 1.0, .8, 1.0);
	specularComponent = vec4(.4, .3, .7, 1.0);
	material_ambient = vec4(1.0, 0.8, 0.1, 1.0);
	material_diffuse = vec4(.3, 0.4, 0.3, 1.0);
	material_specular = vec4(1.0, 1.0, 1.0, 1.0);
	ambient = glGetUniformLocation(program, "Ambient");
	diffuse = glGetUniformLocation(program, "Diffuse");
	specular = glGetUniformLocation(program, "Specular");

	glUniform4fv(ambient, 1, ambientComponent * material_ambient);
	glUniform4fv(diffuse, 1, diffuseComponent * material_diffuse);
	glUniform4fv(specular, 1, specularComponent * material_specular);
	glUniform4fv(glGetUniformLocation(program, "LightDirection"), 1,
			lightDirection);
}

//Initializes shaders, buffers, and vertex arrays
void initGeometry() {
	GLuint program = InitShader("Shaders/vertex.glsl", "Shaders/fragment.glsl");
	glUseProgram(program);
	GLuint pos = glGetAttribLocation(program, "vPosition");
	GLuint color = glGetAttribLocation(program, "vColor");
	GLuint normal = glGetAttribLocation(program, "vNormal");
	modelView = glGetUniformLocation(program, "modelView");
	projection = glGetUniformLocation(program, "projection");
	normalMatrix = glGetUniformLocation(program, "normalMatrix");
	useLighting = glGetUniformLocation(program, "useLighting");
	glGenVertexArrays(3, vertexArrays);

	glGenBuffers(2, gridBuffer);
	glGenBuffers(3, tetrominoBuffer);
	glGenBuffers(3, blockBuffer);

	initBuffers(program, pos, color, normal);
	initLighting(program);

	mat4 proj = Ortho(-1, 1, -1, 1, -5, 5);
	glUniformMatrix4fv(projection, 1, GL_TRUE, proj);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Handles special key presses
void specKeyInput(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		tetris->moveTetrominoZ(1);
		break;
	case GLUT_KEY_LEFT:
		//Move left
		tetris->moveTetrominoX(-1);
		break;
	case GLUT_KEY_RIGHT:
		//Move right
		tetris->moveTetrominoX(1);
		break;
	case GLUT_KEY_DOWN:
		//Move down
		tetris->moveTetrominoZ(-1);
		break;
	}
	tetris->logic();
	glutPostRedisplay();
}

//Handles mouse presses
void mouseKeyInput(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		x = x - 250;
		y = y - 250;
		if (x > 0) {
			translationX += .02;
		} else {
			translationX -= .02;
		}

		if (y > 0) {
			translationY -= .02;
		} else {
			translationY += .02;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		x = x - 250;
		y = y - 250;
		rotationY -= 10 * x / 250;
		rotationX -= 10 * y / 250;

		if (rotationY < -360.0) {
			rotationY = rotationY + 360.0;
		} else if (rotationY > 360.0) {
			rotationY = rotationY - 360.0;
		}

		if (rotationX < -360.0) {
			rotationX = rotationX + 360.0;
		} else if (rotationX > 360.0) {
			rotationY = rotationX - 360.0;
		}
		mat4 nMatrix = Scale(.08, .08, .08);
		nMatrix = RotateX(rotationX) * nMatrix;
		nMatrix = RotateY(rotationY) * nMatrix;

		glUniformMatrix4fv(normalMatrix, 1, GL_TRUE, nMatrix);
	}
	glutPostRedisplay();
}

//Changes the perspective between orthographic and perspective
void togglePerspective() {
	mat4 proj;
	if (projType == P_ORTHO) {
		proj = Perspective(45.0, 1, .5, -1);
		scale = .04;
		projType = P_PERS;
	} else {
		proj = Ortho(-1, 1, -1, 1, -5, 5);
		scale = .08;
		projType = P_ORTHO;
	}
	glUniformMatrix4fv(projection, 1, GL_TRUE, proj);
}

//Handles key presses
void keyInput(unsigned char key, int x, int y) {

	switch (key) {
	case ' ':
		tetris->moveTetrominoBottom();
		break;
	case 'b':
	case 'B':
		if (drawSphere) {
			drawSphere = false;
			showLighting = 1;
		} else {
			drawSphere = true;
			showLighting = 0;
		}
		break;
	case 'f':
	case 'F':
		if (shadingType == FLAT_SHADING) {
			shadingType = GOURAUD_SHADING;
		} else {
			shadingType = FLAT_SHADING;
		}
		break;
	case 'g':
	case 'G':
		vg.toggleGridType();
		break;
	case 'p':
	case 'P':
		togglePerspective();
		break;
	case 'q':
	case 'Q':
		//Quit
		exit(0);
		break;
	case 'x':
		tetris->rotateTetromino(CCW, ROTATE_X);
		break;
	case 'X':
		tetris->rotateTetromino(CW, ROTATE_X);
		break;
	case 'y':
		tetris->rotateTetromino(CCW, ROTATE_Y);
		break;
	case 'Y':
		tetris->rotateTetromino(CW, ROTATE_Y);
		break;
	case 'z':
		tetris->rotateTetromino(CCW, ROTATE_Z);
		break;
	case 'Z':
		tetris->rotateTetromino(CW, ROTATE_Z);
		break;
	case '8':
		//Move up
		tetris->moveTetrominoZ(1);
		break;
	case '4':
		//Move left
		tetris->moveTetrominoX(-1);
		break;
	case '6':
		//Move right
		tetris->moveTetrominoX(1);
		break;
	case '2':
		//Move down
		tetris->moveTetrominoZ(-1);
		break;
	case 's':
		scale += .005;
		if (scale > .20) {
			scale = .20;
		}
		break;
	case 'S':
		scale -= .005;
		if (scale < .02) {
			scale = .02;
		}
		break;
	}
	tetris->logic();
	glutPostRedisplay();
}

//Draws the falling tetromino
void drawTetromino() {
	if (tetris->isTetroFalling()) {
		int size = 0, i;
		GLenum drawType;
		mat4 transformation;
		if (tetris->getTetroTrans(transformation)) {
			transformation = calcTransformationMatrix() * transformation;
		} else {
			transformation = calcTransformationMatrix();
		}

		glBindVertexArray(vertexArrays[1]);
		if (drawSphere) {
			int i;
			vec4 buffer[4];
			tetris->getTetroCenters(buffer);
			for (i = 0; i < T_BLOCKS; i++) {
				createSphere(tetroPoints, size, size, buffer[i], tetroColor,
						tetris->currTetro->color);
				glUniform1f(useLighting, showLighting);
			}
		} else {
			tetris->getTetro(tetroPoints, tetroColor);
			size = T_POINTS;
			normalize(tetroNormals, tetroPoints, size);
			glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[2]);
			glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float),
					tetroNormals, GL_DYNAMIC_DRAW);
			glUniform1f(useLighting, showLighting);
		}

		glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), tetroPoints,
				GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[1]);
		glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), tetroColor,
				GL_DYNAMIC_DRAW);

		glUniformMatrix4fv(modelView, 1, GL_TRUE, transformation);
		if (!drawSphere) {
			glDrawArrays(GL_TRIANGLES, 0, T_POINTS);
			mat4 shadowTranslate = Translate(0, tetris->getMovementToBottom()
					+ 1, 0);
			tetrominoTranslation(shadowTranslate, tetroPoints, size, tetroColor);
			glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[0]);
			glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float),
					tetroPoints, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, tetrominoBuffer[1]);
			glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), tetroColor,
					GL_DYNAMIC_DRAW);
			glUniformMatrix4fv(modelView, 1, GL_TRUE,
					calcTransformationMatrix());
			glUniform1f(useLighting, 0);
			glDrawArrays(GL_TRIANGLES, 0, T_POINTS);
		} else {
			glDrawArrays(GL_TRIANGLE_STRIP, 0, size);
		}
	}
}

//Draws the fallen tetromino blocks
void drawBlocks() {
	int size = 0, i, numOfBlocks;
	GLenum drawType;
	glBindVertexArray(vertexArrays[2]);
	if (drawSphere) {
		int i;
		vec4 t_points[GRID_ROW * GRID_COL * GRID_DEPTH];
		vec4 t_color[GRID_ROW * GRID_COL * GRID_DEPTH];
		tetris->getGridCenters(t_points, t_color, numOfBlocks);
		for (i = 0; i < numOfBlocks; i++) {
			createSphere(blockPoints, size, size, t_points[i], blockColors,
					t_color[i]);
		}
		glUniform1f(useLighting, showLighting);
		drawType = GL_TRIANGLE_STRIP;
	} else {
		tetris->getBlocks(blockPoints, blockColors, size);
		normalize(blockNormals, blockPoints, (float) (size));
		glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[2]);
		glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), blockNormals,
				GL_DYNAMIC_DRAW);
		glUniform1f(useLighting, showLighting);
		drawType = GL_TRIANGLES;
	}

	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), blockPoints,
			GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(float), blockColors,
			GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(modelView, 1, GL_TRUE, calcTransformationMatrix());
	glDrawArrays(drawType, 0, size);

}

//Draws the wireframe of the grid
void drawGrid() {

	glBindVertexArray(vertexArrays[0]);

	mat4 mv = Translate(-5, -10, -5);
	mv = Scale(scale, scale, scale) * mv;
	mv = Translate(translationX, translationY, 0) * mv;
	mv = RotateX(rotationX) * mv;
	mv = RotateY(rotationY) * mv;

	glBindBuffer(GL_ARRAY_BUFFER, gridBuffer[0]);
	if (vg.getGridType() == VG_HALF_WIRE_FRAME) {
		vg.calculatePartialGrid(rotationX, rotationY);
		glBufferData(GL_ARRAY_BUFFER, 4 * vg.getSize() * sizeof(float),
				vg.partialGrid, GL_DYNAMIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, 4 * vg.getSize() * sizeof(float),
				vg.FullGrid, GL_DYNAMIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, gridBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * vg.getSize() * sizeof(float),
			vg.colorBuffer, GL_DYNAMIC_DRAW);
	glUniformMatrix4fv(modelView, 1, GL_TRUE, calcTransformationMatrix());
	glUniform1f(useLighting, 0);

	glDrawArrays(GL_LINES, 0, vg.getSize());
}

//Draw to screen
void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrid();
	drawBlocks();
	drawTetromino();
	glFlush();
}

void timer(int value) {
	//Move tetromino down only once every 500 ms
	static int gravityTimer = 0;
	if (gravityTimer >= 300) {
		gravityTimer = 0;
		tetris->moveTetrominoY(-1);
	} else {
		gravityTimer += 50;
	}
	tetris->logic();
	char currScore[32];
	int s = tetris->getScore();
	sprintf(currScore, "%d", s);
	score->set_text(currScore);
	if (tetris->isGameOver()) {
		status->set_text("Game Over");
	} else {
		status->set_text("Running");
	}

	glutSetWindow(window);
	glutPostRedisplay();
	glutTimerFunc(50, timer, 0);
}

void reshape(int w, int h) {
	GLUI_Master.auto_set_viewport();
}

//Initializes OpenGL and GLUT
void initOpenGL() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(CANVAS + 182, CANVAS);

	glutInitContextVersion(2, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	window = glutCreateWindow("Tetris");
	glewExperimental = GL_TRUE;
	glewInit();

	glutDisplayFunc(displayFunc);
	glutTimerFunc(500, timer, 0);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specKeyInput);
	glutMouseFunc(mouseKeyInput);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

//Executes user input
void guiInput(int option) {
	if (option == 0) {
		glUniform4fv(ambient, 1, ambientComponent * material_ambient);
		glUniform4fv(diffuse, 1, diffuseComponent * material_diffuse);
		glUniform4fv(specular, 1, specularComponent * material_specular);
	} else if (option == 1) {
		tetris->reset();
	} else if (option == 2) {
		exit(0);
	}
	glutSetWindow(window);

}

//Initializes the GUI
void initGLUI() {
	GLUI_Master.set_glutMouseFunc(mouseKeyInput);
	GLUI_Master.set_glutSpecialFunc(specKeyInput);
	GLUI_Master.set_glutKeyboardFunc(keyInput);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutIdleFunc(NULL);
	GLUI *glui_subwin = GLUI_Master.create_glui_subwindow(window,
			GLUI_SUBWINDOW_RIGHT);

	GLUI_Panel *buttons = glui_subwin->add_panel("");

	//Create new and quit game buttons
	glui_subwin->add_button_to_panel(buttons, "New Game", 1, guiInput);
	glui_subwin->add_button_to_panel(buttons, "Quit", 2, guiInput);

	//Create score and game status control
	GLUI_Panel *GameStatus = glui_subwin->add_panel("Game Status");
	GameStatus->set_w(500);
	score = glui_subwin->add_edittext_to_panel(GameStatus, "Score:");
	status = glui_subwin->add_edittext_to_panel(GameStatus, "Status:");

	GLUI_Panel *settings = glui_subwin->add_panel("Game Settings");
	glui_subwin->add_checkbox_to_panel(settings, "Enable Lighting",
			&showLighting);

	glui_subwin->add_separator_to_panel(settings);

	GLUI_Spinner *spinner;
	glui_subwin->add_statictext_to_panel(settings, "Ambient");
	spinner = glui_subwin->add_spinner_to_panel(settings, "R",
			GLUI_SPINNER_FLOAT, &ambientComponent[0], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "G",
			GLUI_SPINNER_FLOAT, &ambientComponent[1], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "B",
			GLUI_SPINNER_FLOAT, &ambientComponent[2], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);

	glui_subwin->add_statictext_to_panel(settings, "Diffuse");
	spinner = glui_subwin->add_spinner_to_panel(settings, "R",
			GLUI_SPINNER_FLOAT, &diffuseComponent[0], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "G",
			GLUI_SPINNER_FLOAT, &diffuseComponent[1], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "B",
			GLUI_SPINNER_FLOAT, &diffuseComponent[2], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);

	glui_subwin->add_statictext_to_panel(settings, "Specular");
	spinner = glui_subwin->add_spinner_to_panel(settings, "R",
			GLUI_SPINNER_FLOAT, &specularComponent[0], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "G",
			GLUI_SPINNER_FLOAT, &specularComponent[1], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);
	spinner = glui_subwin->add_spinner_to_panel(settings, "B",
			GLUI_SPINNER_FLOAT, &specularComponent[2], 0, guiInput);
	spinner->set_float_limits(0.0f, 1.0f);

	glui_subwin->set_main_gfx_window(window);
}

//Initializes all view settings
void initView(Game *game, int argc, char ** argv) {
	glutInit(&argc, argv);
	tetris = game;
	initOpenGL();
	initGeometry();
	initGLUI();
	glutMainLoop();
}

