#pragma once
#include "Obj.h"
#include "Bullet.h"

#include <map>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <irrKlang.h>

class Player :
	public Obj
{
private:
	int bIsRolling = 0;
	bool bIsLooping = false;
	bool bIsFiring = false;
	float fCannonCool = 3.f;
	float fMaxAngle = glm::pi<float>() / 2.f;
	float maxBurnTime = 10.f;

	float LoopTimer = 0.f;
	float RollTimer = 0.f;
	float FireTimer = 0.f;
	
public:
	std::vector<Bullet> Bullets;
	Texture bulletTex = Texture("textures\\ZoneGate.png");
	Model bulletModel = Model("models\\sphere.obj");
	irrklang::ISoundEngine* soundEngine;

	Player();
	Player(Texture &texture, Model &model, int collider);
	void Update(std::map<int, bool> Input, std::map<int, bool> Prev, float delt);
	void FireMainCannons();
	void Loop();
	void Roll(int direction);
	virtual ~Player();


};

