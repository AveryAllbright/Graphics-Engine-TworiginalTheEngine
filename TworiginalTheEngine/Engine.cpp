#include "Engine.h"

Timer Engine::time;
ShaderManager Engine::ShadMan;

Engine::Engine()
{
	//Nothing to do here
}

//Engine Startup Procedure
bool Engine::Init()
{
	glfwInit(); //Initialize GLFW to allow window creation
	glfwWindowHint(GLFW_SAMPLES, 4); //MultiSample (4) to enable anti-aliasing
	glEnable(GL_MULTISAMPLE); //Enable MultiSample
	pWindow = glfwCreateWindow(1200, 850, "Banks DSA1 Engine", NULL, NULL); //Create a new window

	if (pWindow == nullptr)
	{
		glfwTerminate();	//end init
		return false;		//Report Failure
	}
	glfwMakeContextCurrent(pWindow);

	if (glewInit() == GLEW_OK)
	{
		return true;	//Engine Init OK, Report Success
	}

	glfwTerminate();	//end Init
	return false;		//Report Failure
}

//Set Polymode and Culling defaults
void Engine::BufferModels()
{
	glEnable(GL_DEPTH_TEST);			//Don't bother overwriting framebuffer if fragment is further away
	glEnable(GL_CULL_FACE);				//Enable Culling
	glPolygonMode(GL_FRONT, GL_FILL);	//Render Fronts, fill style
}

//Set values and definitions for engine-side objects
void Engine::Setup()
{
	fZoom = 1.0f; //zoom standard, 100%
	glfwGetWindowSize(pWindow, &nWindowWidth, &nWindowHeight); //Get window size

	//Set action callbacks for input
	glfwSetKeyCallback(pWindow, keyCallback);
	glfwSetMouseButtonCallback(pWindow, mouseClick);

	keyInput = keyIsDown;
	
	glClearColor(.1, .35, 1, 1); //Set Background : Not really important when you have a skybox

	time.Start();
}

//Update Engine Side 'Objects' : Timer, Input Control
void Engine::Update()
{
	time.Update();
	keyWasDown = keyIsDown;
	Prev = keyInput;
	glfwPollEvents();
	keyInput = keyIsDown;
}

//Clear Renderer
void Engine::Render()
{
	glfwSwapBuffers(pWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Returns true if ALL shaders load properly
bool Engine::UseShaders()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (ShadMan.LoadShaders(vertFile, fragFile, &outShaderLoc) && ShadMan.LoadShaders(skyVert, skyFrag, &skyShaderLoc))
	{
		return true;
	}

	return false;
}

Engine::~Engine()
{
	//Nothing to do here
}
