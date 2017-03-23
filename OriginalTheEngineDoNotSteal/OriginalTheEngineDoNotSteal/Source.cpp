#include <iostream>
#include "Engine.h"

int main()
{
	std::cout << "Hello World\n";
		
	Engine engine; //Declare an engine

	if (!engine.init()) //Ensure engine runs before continuing
	{
		return -1;
	}

	if (!engine.BufferModel()) //Ensure everything is loaded onto the screen correctly before continuing. RETURN: engine currently renders a built in shape, always true
	{
		return -1;
	}

	if (engine.UseShaders()) //Ensure shader files load properly before continuing
	{
		engine.GameLoop(); //Loop until finished
	}

	std::cout << "\nGoodbye World";
	return 0;
}