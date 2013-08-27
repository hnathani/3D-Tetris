#include "Angel.h"

#ifndef GL_VERSION_3_2
#error "OpenGL >= 3.2 required"
#endif // GL_VERSION_3_2

GLuint  model_view; // model-view matrix uniform shader variable location
GLuint  projection; // projection matrix uniform shader variable location


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// This part of the file deals with the loading of the shaders. This has
// been copied from the code Ed Angel provides with the 6th edition of his book:
// "Interactive Computer Graphics, A top-down approach with Shader-based OpenGL".
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "r");

    if ( fp == NULL ) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint
Angel::InitShader(const char* vShaderFile, const char* fShaderFile)
{
    struct Shader {
	const char*  filename;
	GLenum       type;
	GLchar*      source;
    }  shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();
    
    for ( int i = 0; i < 2; ++i ) {
	Shader& s = shaders[i];
	s.source = readShaderSource( s.filename );
	if ( shaders[i].source == NULL ) {
	    std::cerr << "Failed to read " << s.filename << std::endl;
	    exit( EXIT_FAILURE );
	}

	GLuint shader = glCreateShader( s.type );
	glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
	glCompileShader( shader );

	GLint  compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
	    std::cerr << s.filename << " failed to compile:" << std::endl;
	    GLint  logSize;
	    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
	    char* logMsg = new char[logSize];
	    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
	    std::cerr << logMsg << std::endl;
	    delete [] logMsg;

	    exit( EXIT_FAILURE );
	}

	delete [] s.source;

	glAttachShader( program, shader );
    }

    /* link  and error check */
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
	std::cerr << "Shader program failed to link" << std::endl;
	GLint  logSize;
	glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetProgramInfoLog( program, logSize, NULL, logMsg );
	std::cerr << logMsg << std::endl;
	delete [] logMsg;

	exit( EXIT_FAILURE );
    }

    /* use program object */
    glUseProgram(program);

    return program;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// setup of the callback functions / setup of the windowing stuff
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void ReshapeFunc (int w, int h)
{
	mat4 p;
	
	// Sets up the projection matrix, the current projection chosen is an
	// orthogonal projection

	glViewport(0, 0, w, h); 
	if (h >= w) 
		p = Ortho2D(-1.0, 1.0,
				-1.0 * (GLfloat) h / (GLfloat) w, 
				1.0 * (GLfloat) h / (GLfloat) w);
	else
		p = Ortho2D(-1.0 * (GLfloat) w / (GLfloat) h, 
				1.0 * (GLfloat) w / (GLfloat) h, -1.0, 1.0); 

	// send the new projection matrix to the shader
	glUniformMatrix4fv( projection, 1, GL_TRUE, p );
	
	// Ask glut to call the drawing function (DisplayFunc)
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
void DisplayFunc ( void )
{
	// clear the drawing window
	glClear(GL_COLOR_BUFFER_BIT); 

	// Applying a transformation
	mat4 mv = Translate(0.3, 0.3, 0.0) * Scale(0.5, 0.5, 0.5);

	// send the new model-view matrix to the shader
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	// draw the quad
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	// swap the buffer
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
void initOpenGL ( void )
{
	// Request the kind of rendering context we want:
	// in this case i am requestiong a double buffered screen,
	// and a pixel array that stores rgba information
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);

	// Tells glut about the size of the window it should open
	glutInitWindowSize (500, 500);

	// The next two lines will check if the code is truly 3.2
	glutInitContextVersion(3,2);
	glutInitContextProfile( GLUT_CORE_PROFILE );

	// Finally create the rendering context
	glutCreateWindow ("Simple OpenGL 3.2 program");

	// glew takes care of compatibility across platforms
	glewExperimental = GL_TRUE;
	glewInit();

   // Register some call back function required by glut to
   // refresh and resize the screen
   glutDisplayFunc (DisplayFunc);
   glutReshapeFunc (ReshapeFunc);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// setup of the Geometry and the shaders
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void initGeometry ( void )
{
    // Specifiy the vertices for a quad
    vec2 quad[4] = {
        vec2(-1.0, 1.0), vec2( -1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( 1.0, -1.0 )
    };

    // Create a vertex array object
    GLuint vao[1];
    glGenVertexArrays( 1, vao );
	glBindVertexArray( vao[0] );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vSimpleR.glsl", "fSimple.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

	projection = glGetUniformLocation( program, "projection" );
	model_view = glGetUniformLocation( program, "model_view" );
	
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// finally the main function
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


int main (int argc, char ** argv)
{
	glutInit (&argc, argv);
	
	// Initialize the OpenGL states and set the callback functions
	initOpenGL();

	// Initialize the geometry and shaders
	initGeometry();
	
    printf("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

	/* OpenGL main loop */
	glutMainLoop();

	return 0;
}

