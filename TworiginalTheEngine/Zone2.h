#pragma once
#include "Scene.h"
class Zone2 :
	public Scene
{
public:
	Obj Apology;
	Model ApologyModel = Model("models\\board.obj");
	Texture ApologyTexture = Texture("textures\\Sorry.png");
	Camera Fly;

	Zone2();
	Zone2(GLFWwindow * window, int shad);
	void PreGame() override;
	void Update(float delt);
	void Render();

	virtual ~Zone2();
};

