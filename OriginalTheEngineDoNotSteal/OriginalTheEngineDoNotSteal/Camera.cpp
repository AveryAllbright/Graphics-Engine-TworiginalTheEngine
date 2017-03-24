#include "Camera.h"
#include <iostream>
Camera::Camera()
{
	//Nothing to do here
}

Camera::Camera(float fovy, float aspect, float zNear, float zFar, GLFWwindow* windowPtr) : fFovy(fovy), fAspect(aspect), fzNear(zNear), fzFar(zFar)
{
	time = 0;
	delt = 0;
	prevTime = 0;
	perpMat = glm::perspective(fFovy, fAspect, fzNear, fzFar);
	vCamLoc = { 0,0,2 };
	vCamRot = { 0,0,0,1 };
	sens = .0005;
	window = windowPtr;
	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w / 2, h / 2);

	rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);

	eye = vCamLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);

	lookMat = glm::lookAt(eye, center, up);

	camMat = perpMat * lookMat;
}

void Camera::Update(std::map<int, bool> input, std::map<int, bool> prev)
{
	inputHandling = input;
	prevTime = time;
	time = (float)glfwGetTime();
	delt = time - prevTime;


	glfwGetCursorPos(window, &x, &y);
	vCamRot.y -= sens * (x - w * .5f);
	vCamRot.x -= sens * (y - h * .5f);
	vCamRot.x = glm::clamp(vCamRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());
	vCamRot.y = glm::clamp(vCamRot.y, -.25f * glm::pi<float>(), .25f * glm::pi<float>());

	rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);
	if (inputHandling != prev) { delt = 0; }
	if (inputHandling[GLFW_KEY_LEFT]) { vCamVel += rotMat * glm::vec3(-1, 0, 0); }
	if (inputHandling[GLFW_KEY_RIGHT]) { vCamVel += rotMat * glm::vec3(1, 0, 0); }
	if (inputHandling[GLFW_KEY_UP]) { vCamVel += rotMat * glm::vec3(0, 0, -1); }
	if (inputHandling[GLFW_KEY_DOWN]) { vCamVel += rotMat * glm::vec3(0, 0, 1); }
	if (inputHandling[GLFW_KEY_SPACE]) { vCamVel += rotMat * glm::vec3(0, 1, 0); }
	if (inputHandling[GLFW_KEY_LEFT_SHIFT]) { vCamVel += rotMat * glm::vec3(0, -1, 0); }

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
