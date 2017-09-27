#include "Timer.h"
#include <GLFW\glfw3.h>

Timer::Timer()
{
}

//Begin timer at full 0
void Timer::Start()
{
	fTime = fDelt = FPS = fPrev = 0;
}

//Update delta Time
void Timer::Update()
{
	fPrev = fTime;
	fTime = glfwGetTime();
	fDelt = fTime - fPrev;
	FPS = 1 / fDelt;
}

Timer::~Timer()
{
}
