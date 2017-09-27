#pragma once
#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <map>

#include "Obj.h"
class Camera
{
private:
	float fFovy;
	float fAspect;
	float zNear;
	float zFar;

	float fSens;
	float fMaxSpeed;
	float fDelt;

	int w, h;
	double x, y;
	float theta, phi;
	float radius = 15.f;

	glm::vec3 vCamLoc;
	glm::vec3 vCamVel;
	glm::vec3 vCamAcc;
	glm::vec4 vCamRot;

	glm::mat4 perpMat;
	glm::mat3 rotMat;
	glm::mat4 camMat;
	glm::mat4 lookMat;

	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;


	GLFWwindow* pWindow;
	std::map<int, bool>Input;
public:
	char* type;

	Camera();
	Camera(float fovy, float aspect, float zNear, float zFar, GLFWwindow* windowPtr, char* tag);
	void Update(float delt, std::map<int, bool>Input);
	void Update(float delt, Obj &target);
	void Rotation();
	void Movement();
	void Upload();
	virtual ~Camera();
	void setLoc(glm::vec3 loc);
	glm::vec3 getLoc();
};

