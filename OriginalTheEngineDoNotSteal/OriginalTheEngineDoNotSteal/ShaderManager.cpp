#include "ShaderManager.h"
#include <fstream>
#include <iostream>

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream inFile(file, std::ios::binary);
	std::string input;
	if (!inFile.is_open())
	{
		std::cout << "Cannot load file properly";
		return 0;
	}

	inFile.seekg(0, std::ios::end);
	int length = (int)inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	char* fileContents = new char[length + 1];

	inFile.read(fileContents, length);
	fileContents[length] = 0;
	inFile.close();	

	GLint index = glCreateShader(shaderType);
	glShaderSource(index, 1, &fileContents, NULL);
	glCompileShader(index);

	delete[] fileContents;

	GLint success = GL_FALSE;
	glGetShaderiv(index, GL_COMPILE_STATUS, &success);
	if (success != 0) { return index; }
	else
	{
		GLint failure = GL_FALSE;
		glGetShaderiv(index, GL_INFO_LOG_LENGTH, &failure);
		GLchar* logger = new GLchar[failure];
		glGetShaderInfoLog(index, failure, 0, logger);
		std::cout << logger;

		delete[] logger;
		return 0;
	}

}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile, GLuint* outShaderLoc)
{
	GLuint vert = 0;
	GLuint frag = 0;

	vert = loadShader(vertexFile, GL_VERTEX_SHADER);
	frag = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if (vert == 0 || frag == 0)
	{
		return false;
	}
	
	*outShaderLoc = glCreateProgram();
	glAttachShader(*outShaderLoc, vert);
	glAttachShader(*outShaderLoc, frag);
	glLinkProgram(*outShaderLoc);

	GLint success = 0;
	glGetProgramiv(*outShaderLoc, GL_LINK_STATUS, &success);
	if (success != 0) { return true; }

	GLint failure = 0;
	glGetProgramiv(*outShaderLoc, GL_INFO_LOG_LENGTH, &failure);
	GLchar* logger = new GLchar[failure];
	glGetProgramInfoLog(*outShaderLoc, failure, 0, logger);
	std::cout << logger << std::endl;

	delete[] logger;
	return false;
}