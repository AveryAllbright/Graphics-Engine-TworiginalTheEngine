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
	float prev;
	float time;
	float delt;
	float sens;
	int w, h;
	glm::vec3 vCamLoc;
	glm::vec3 vCamVel;
	glm::vec4 vCamRot;
	glm::mat4 perpMat;
	glm::mat4 lookMat;

	glm::mat3 rotMat;
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	
	GLFWwindow* GLFWwindowPtr;
	std::map<int, bool> inputHandling;
public:
	glm::mat4 camMat;
	double x, y;
	Camera();
	Camera(float fovy, float aspect, float zNear, float zFar);
	void setAspects(int width, int height);
	void Update(std::map<int, bool> input);
	virtual ~Camera();
};

