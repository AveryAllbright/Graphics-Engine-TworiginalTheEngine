#include "Engine.h"
#include <glm\matrix.hpp>
#include <map>
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

Engine::Engine()
{
	//Nothing to do here
}

///Engine startup procedure
bool Engine::init()
{
	glfwInit(); //Initialize GLFW to allow window creation

	GLFWwindowPtr = glfwCreateWindow(1200, 850, "Banks DSA1 Engine", NULL, NULL); //Creates a new window 

	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr); //Sets the active window to the created one, if it exists
	}
	else
	{
		glfwTerminate(); //end init
		return false;
	}

	if (glewInit() != GLEW_OK) //Initializes GLEW and ensures it ran correctly
	{
		glfwTerminate();
		return false;
	}

	return true;
}

///Currently: Creates shape on screen
bool Engine::BufferModel()
{
	std::vector<glm::vec3> locs =  // - 1, 1 coordinates in the window
	{
		{ .9, .9, 0 },
		{ -.9, .9, 0 },
		{ -.9, -.9, 0 },
		{ .9, -.9, 0 }
	};

	std::vector <unsigned int>
		locInds =				//Creates the pair of triangles that makes up a quad
	{ 0,1,2,			// points 0, 1, 2 make a triangle
		0,2,3 };			// and so do points 0, 2, and 3

	vertCount = locInds.size();		//tracks how many vertices exist (4)

	GLuint vertBuf;

	std::vector<glm::vec3> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = locs[locInds[i]];
	}

	glGenVertexArrays(1, &vertArr); //generates the active arrays (in this case, just the one we created)
	glGenBuffers(1, &vertBuf);		//generates the active buffers (in this case, just the one we created)
	
	glBindVertexArray(vertArr);				//Binds the vertexArray we created as the active array
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf); //Binds the buffer we created as the active buffer

	glBufferData(GL_ARRAY_BUFFER,		// Where to copy to
		sizeof(glm::vec3) *vertCount,	// how many bytes to copy
		&vertBufData[0],				// Where to copy from
		GL_STATIC_DRAW);				// "hint" to OpenGL (static draw means it is the quickest possible, buffer knows it will never have new data

	glEnableVertexAttribArray(0);	// Attribute index 0

	glVertexAttribPointer(
		0,					//Attribute index 
		3,					//Number of components (x, y, z)
		GL_FLOAT,			//Type of data
		GL_FALSE,			//Data normalization?
		sizeof(glm::vec3),	//Stride (bytes per vertex) 
		0);					//Offset to this attribute

	glBindVertexArray(0); //unbind when finished editing
	
	return true;
}

///Game loops 
bool Engine::GameLoop()
{
	//Set default window values
	zoom = 1.0f;
	width = 1200;
	height = 850;

	//Set background 
	red = rgbToFloat(112);
	green = rgbToFloat(128);
	blue = rgbToFloat(144);
	glClearColor(red, green, blue, 1.0f); //Slate Grey

	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	
	float fovy = 3.14159f * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.0f;
	Camera camera(fovy, aspect, zNear, zFar);
	camera.setAspects(width, height);

	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		glClear(GL_COLOR_BUFFER_BIT); //Clears the Screen
		double randVal = rand() % 1;
		float fRand = randVal / 10;

		//Green/Magenta Pulse
		GLfloat time = glfwGetTime();
		GLfloat redValue = (sin(time) / 2) + fRand;
		GLfloat greenValue = (cos(time) / 2) + fRand;
		GLfloat blueValue = (sin(time) / 2) + fRand;
		GLfloat alphaValue = (cos(time) / 2) + .5f;
		glUniform4f(3, redValue, greenValue, blueValue, alphaValue);

		//Key input updates
		keyWasDown = keyIsDown;
		glfwPollEvents();

		//Camera Update
		glfwGetCursorPos(GLFWwindowPtr, &camera.x, &camera.y);
		camera.Update(keyIsDown);
		glBindVertexArray(vertArr);
		glDrawArrays(GL_TRIANGLES, 0, vertCount); // draw triangles, from 0th index to index of (size of array holding triangles)
		glBindVertexArray(0); // unbind an object after drawing it
		
		//Swap the front (display) and back (what GL draws to) buffers
		glfwSwapBuffers(GLFWwindowPtr);
		
	}
	glfwTerminate();
	return true;
}

bool Engine::UseShaders()
{
	GLuint outShader;
	GLuint* outShaderLoc = &outShader;
	if (shadMan.loadShaders("shaders\\vShader.glsl", "shaders\\fShader.glsl", outShaderLoc))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(*outShaderLoc);
		return true;
	}
	return false;
}

float Engine::rgbToFloat(int rgbValue)
{
	return ((float)rgbValue / 255.0);
}


Engine::~Engine()
{
}
