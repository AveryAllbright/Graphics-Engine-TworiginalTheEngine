#include "ShaderManager.h"

//Pull Shader from File to load into program
GLuint ShaderManager::LoadShader(const char * file, GLenum shaderType)
{
	//Open File in Binary mode
	std::ifstream inFile(file, std::ios::binary);
	std::string sInput;

	//Write to ErrorMessage if file fails to load
	if (!inFile.is_open())
	{
		OutFile.open("FatalError.txt");
		OutFile << "Fatal Error : Could not load Shader file: " << file << " properly.";
		OutFile.close();
	}

	//Find the end of the file, denote the length, and then move back to the front
	inFile.seekg(0, std::ios::end);
	int nLength = (int)inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	char* fileContents = new char[nLength + 1];

	//Read Shader File
	inFile.read(fileContents, nLength);
	fileContents[nLength] = 0;
	inFile.close();

	//Write a glShader from the file
	GLint nIndex = glCreateShader(shaderType);
	glShaderSource(nIndex, 1, &fileContents, NULL);
	glCompileShader(nIndex);

	//Delete the file
	delete[] fileContents;

	//Check to ensure the Shader compiled
	GLint nSuccess = GL_FALSE;
	glGetShaderiv(nIndex, GL_COMPILE_STATUS, &nSuccess);
	if (nSuccess == 0)
	{
		//Grab the error message from the info log and output it as an error message
		GLint nFailure = GL_FALSE;
		glGetShaderiv(nIndex, GL_INFO_LOG_LENGTH, &nFailure);
		GLchar* logger = new GLchar[nFailure];
		glGetShaderInfoLog(nIndex, nFailure, 0, logger);
		OutFile.open("FatalError.txt");
		OutFile << logger;
		OutFile.close();
		delete[] logger;
		return 0;
	}

	//return a non-zero integer if the shader compiled correctly
	return nIndex;
}

//Compile Vert and Frag Shaders into a Shader Program
bool ShaderManager::LoadShaders(const char * vertexFile, const char * fragmentFile, GLuint* outShaderLoc)
{
	GLuint nVertLoc = 0;
	GLuint nFragLoc = 0;

	//Find the GPU locations for each half of the shader
	nVertLoc = LoadShader(vertexFile, GL_VERTEX_SHADER);
	nFragLoc = LoadShader(fragmentFile, GL_FRAGMENT_SHADER);

	//Check that the files exist...
	if (nVertLoc != 0 && nFragLoc != 0)
	{
		//... and link them into a Shader Program
		*outShaderLoc = glCreateProgram();
		glAttachShader(*outShaderLoc, nVertLoc);
		glAttachShader(*outShaderLoc, nFragLoc);
		glLinkProgram(*outShaderLoc);

		GLint nSuccess = 0;
		glGetProgramiv(*outShaderLoc, GL_LINK_STATUS, &nSuccess);
		if (nSuccess != 0) { return true; }

		GLint nFailure = 0;
		glGetProgramiv(*outShaderLoc, GL_INFO_LOG_LENGTH, &nFailure);
		GLchar* logger = new GLchar[nFailure];
		glGetProgramInfoLog(*outShaderLoc, nFailure, 0, logger);
		OutFile.open("FatalError.txt");
		OutFile << logger;
		OutFile.close();
		delete[] logger;
	}
	return false;
}

