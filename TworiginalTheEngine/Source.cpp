#include <iostream>
#include "Engine.h"
#include "Zone1.h"
#include "Zone2.h"

int main()
{
	std::cout << "Hello World\n";
	Engine engine = Engine();
	
	if (!engine.Init()) //Ensure Engine startup is correct before continuing
	{
		return -1;
	}

	if (engine.UseShaders()) //Load Shaders from files and ensure proper use before continuing
	{
		engine.BufferModels(); //Set buffertype for all models to be updated
		engine.Setup();
		Zone1 zone1 = Zone1(engine.pWindow, engine.outShaderLoc, engine.skyShaderLoc);
		Zone2 zone2 = Zone2(engine.pWindow, engine.outShaderLoc);
		zone1.PreGame();
		while (!engine.keyInput[GLFW_KEY_GRAVE_ACCENT]) //Tilde key
		{				
			if (zone1.bIsActiveScene)
			{
				zone1.Update(engine.keyInput, engine.Prev, engine.time.fDelt);
				zone1.Render();
			}
			if (!zone1.bIsActiveScene && !zone2.bIsActiveScene)
			{
				zone2.bIsActiveScene = true;
				zone2.PreGame();
			}
			if (zone2.bIsActiveScene)
			{
				zone2.Update(engine.time.fDelt);
				zone2.Render();
			}
			engine.Update();			//Update Engine level functions 
			engine.Render();	
		}
	}
	glfwTerminate();
	std::cin.get();
	std::cout << "\nGoodbye World";	
	return 0;
}