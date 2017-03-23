#pragma once
#include <FreeImage.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
class ShaderManager
{
public:
	GLuint loadShader(const char* file, GLenum shaderType);
	bool loadShaders(const char* vertexFile, const char* fragmentFile, GLuint* outShaderLoc);
};

