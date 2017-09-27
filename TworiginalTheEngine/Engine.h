#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <map>
#include <vector>

#include "Timer.h"
#include "Obj.h"
#include "Camera.h"
#include "ShaderManager.h"

namespace 
{
	std::map<int, bool> keyIsDown;
	std::map<int, bool> keyWasDown;

	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
	{
		keyIsDown[button] = action;
	}
	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		keyIsDown[key] = action;
	}
}

class Engine
{
private:
	
	//Shader Data
	char* vertFile = "shaders\\vShader.glsl";
	char* fragFile = "shaders\\fShader.glsl";
	char* skyFrag = "shaders\\fSky.glsl";
	char* skyVert = "shaders\\vSky.glsl";
	

	float red;
	float blue;
	float green;

public:
	GLuint outShaderLoc = 0;
	GLuint skyShaderLoc = 0;
	std::map<int, bool> keyInput;
	std::map<int, bool>Prev;
	GLFWwindow* pWindow;
	static Timer time;
	static ShaderManager ShadMan;
	float fZoom;
	int nWindowWidth;
	int nWindowHeight;
	
	Engine();
	bool Init();
	void BufferModels();
	void Setup();
	void Update();
	void Render();
	bool UseShaders();
	virtual ~Engine();
};

