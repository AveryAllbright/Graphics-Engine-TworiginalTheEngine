#pragma once
#include <FreeImage.h>
#include <GL\glew.h>
#include <fstream>
#include <iostream>
class ShaderManager
{
public:
	std::ofstream OutFile;
	GLuint LoadShader(const char* file, GLenum shaderType);
	bool LoadShaders(const char* vertexFile, const char* fragmentFile, GLuint* outShaderLoc);
};

