#include "Camera.h"
#include <iostream>
Camera::Camera()
{
	//Nothing to do here
}

Camera::Camera(float fovy, float aspect, float zNear, float zFar) : fFovy(fovy), fAspect(aspect), fzNear(zNear), fzFar(zFar)
{
	time = 0;
	delt = 0;
	prev = 0;
	perpMat = glm::perspective(fFovy, fAspect, fzNear, fzFar);
	vCamLoc = { 0,0,2 };
	vCamRot = { 0,0,0,1 };

	rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);

	eye = vCamLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);

	lookMat = glm::lookAt(eye, center, up);

	camMat = perpMat * lookMat;
}

void Camera::setAspects(int width, int height)
{
	w = width;
	h = height;
}

void Camera::Update(std::map<int, bool> input)
{
	inputHandling = input;
	prev = time;
	time = (float)glfwGetTime();
	delt = time - prev;

	vCamRot.y -= sens * (x - w * .5f);
	vCamRot.x -= sens * (y - h * .5f);

	rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);
	
	if (inputHandling[GLFW_KEY_LEFT]) { vCamVel += rotMat * glm::vec3(-1, 0, 0); std::cout << "left"; }
	if (inputHandling[GLFW_KEY_RIGHT]) { vCamVel += rotMat * glm::vec3(1, 0, 0); std::cout << "right";}
	if (inputHandling[GLFW_KEY_UP]) { vCamVel += rotMat * glm::vec3(0, 0, -1); std::cout << "up";	}
	if (inputHandling[GLFW_KEY_DOWN]) { vCamVel += rotMat * glm::vec3(0, 0, 1); std::cout << "down"; }
	else { vCamVel = glm::vec3(0, 0, 0); }
	float speed = 1.0f;
	if (vCamVel != glm::vec3())
	{
		vCamVel = glm::normalize(vCamVel) * speed;
	}

	vCamLoc += vCamVel;

	eye = vCamLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);
	camMat = glm::perspective(fFovy, fAspect, fzNear, fzFar) * glm::lookAt(eye, center, up);
	glUniformMatrix4fv(1, 1, GL_FALSE, &camMat[0][0]);
}

Camera::~Camera()
{
}
