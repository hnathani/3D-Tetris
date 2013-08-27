
#ifndef SHADER_H_
#define SHADER_H_

#include "../lib/Angel.h"

static char* readShaderSource(const char* shaderFile);
GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);
#endif /* SHADER_H_ */
