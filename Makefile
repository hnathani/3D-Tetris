all:
	g++ -g -o 3DTetris main.cpp Models/Game.cpp Models/ShapeGen.cpp Models/Tetromino.cpp Models/Grid.cpp Shaders/Shader.cpp View/ViewingGrid.cpp Transformations/TetrominoTransforms.cpp View.cpp Transformations/Sphere.cpp -lglui -lglut -lGLEW -lGL
run:
	./3DTetris
clean:
	rm 3DTetris
