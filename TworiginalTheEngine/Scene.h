#pragma once
#include "Camera.h"
#include "Skybox.h"
#include "Obj.h"
#include "World.h"
#include "Player.h"

#include <vector>
#include <map>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

const int LIGHT_LOCATION = 5;
class Scene
{
public:
	std::vector<Camera> Cameras;
	std::vector<glm::vec3> Lights;
	std::map<int, bool> keyWasDown;
	std::map<int, bool> keyIsDown;
	GLFWwindow* pWindow;
	Skybox sky;
	World SceneSpace;
	bool bIsActiveScene;
	float fDelt;
	int nActCamInd;
	int nTotalCams;
	int shaderLoc;
	int skyLoc;

	Scene();
	virtual void PreGame();
	virtual void Update(std::map<int, bool> prevPut, std::map<int, bool>Input, float delt);
	virtual void Render();
	virtual ~Scene();
};

