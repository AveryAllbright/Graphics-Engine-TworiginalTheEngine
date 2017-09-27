#include "Zone2.h"

Zone2::Zone2()
{
	//Nothing to do here
}

Zone2::Zone2(GLFWwindow * window, int shad)
{
	pWindow = window;
	shaderLoc = shad;
}

void Zone2::PreGame()
{
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);
	float zoom = 1;
	float fovy = glm::pi<float>() * .4f / zoom;
	float aspect = (float)width / (float) height;
	float zNear = .01f;
	float zFar = 100.f;

	 Fly = Camera(fovy, aspect, zNear, zFar, pWindow, "fly");

	 ApologyModel.Buffer();
	 ApologyTexture.LoadTexture();

	 Apology = Obj(&ApologyModel, &ApologyTexture, 0);
	 Apology.transform.location = Fly.getLoc();
	 glUseProgram(shaderLoc);
}

void Zone2::Update(float delt)
{
	Apology.transform.location = Fly.getLoc() + glm::vec3(0, 0, -1);
	std::map<int, bool> stay;
	Fly.Update(delt, stay);
}

void Zone2::Render()
{
	Fly.Upload();
	Apology.texture->UseTexture();
	Apology.Render();
}

Zone2::~Zone2()
{
}
