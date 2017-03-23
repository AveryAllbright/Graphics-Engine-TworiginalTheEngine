#pragma once
#include <vector>
#include "ShaderManager.h"
#include "Camera.h"
#include <FreeImage.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
class Engine
{
private: 
	GLFWwindow* GLFWwindowPtr;
	GLuint vertArr;
	unsigned int vertCount;
	ShaderManager shadMan;
	float red;
	float green;
	float blue;
public:

	float zoom;
	int width;
	int height;

	Engine();
	bool init();
	bool BufferModel();
	bool GameLoop();
	bool UseShaders();
	void LoadControls();
	inline float rgbToFloat(int rgbValue);
	virtual ~Engine();
};

