#pragma once
#include "Engine.h"
#include <map>
class Camera
{
private:
	float fFovy;
	float fAspect;
	float fzNear;
	float fzFar;
	float prevTime;
	float time;
	float delt;
	float sens;
	int w, h;
	double x, y;
	glm::vec3 vCamLoc;
	glm::vec3 vCamVel;
	glm::vec4 vCamRot;
	glm::mat4 perpMat;
	glm::mat4 lookMat;

	glm::mat3 rotMat;
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	
	GLFWwindow* window;
	std::map<int, bool> inputHandling;
public:
	glm::mat4 camMat;
	Camera();
	Camera(float fovy, float aspect, float zNear, float zFar, GLFWwindow* windowPtr);
	void Update(std::map<int, bool> input, std::map<int, bool> prev);
	virtual ~Camera();
};

