#include "Camera.h"

const GLuint CAMERA_LOCATION = 6;

Camera::Camera()
{
	//Nothing to do here
}

Camera::Camera(float fovy, float aspect, float zNear, float zFar, GLFWwindow * windowPtr, char* tag) : fFovy(fovy), fAspect(aspect), zNear(zNear), zFar(zFar), type(tag)
{
	perpMat = glm::perspective(fFovy, fAspect, this->zNear, this->zFar); //sets the perspective of the camera using the field of view, aspect ratio, and the near and far render distances
	vCamLoc = vCamVel = vCamAcc = { 0,0,0 };
	vCamVel = { 1,1,1 };
	fSens = .001f;
	fMaxSpeed = 25.f;
	pWindow = windowPtr;
	glfwGetWindowSize(pWindow, &w, &h);
	glfwSetCursorPos(pWindow, w / 2.f, h / 2.f);
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

//Updates Fly type cameras, which move independent of any other objects
void Camera::Update(float delt, std::map<int, bool>Input)
{
	this->Input = Input;
	fDelt = delt;
	Rotation();
	Movement();
	eye = vCamLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);
	lookMat = glm::lookAt(eye, center, up);
	camMat = perpMat * lookMat;
}

//Updates Orbit type cameras, which move at a certain radius to their target object
void Camera::Update(float delt, Obj &target)
{
	fDelt = delt;
	Rotation();
	
	auto forward = (rotMat) * glm::vec3(0,0,-1);
	auto position = target.transform.location - (forward * radius);
	vCamLoc = position;
	lookMat = glm::lookAt(position, forward,  glm::vec3(0, 1, 0));
	camMat = perpMat * lookMat;
}

void Camera::Rotation()
{
	if (type == "fly") //rotates about itself, using the mouse position
	{
		glfwGetCursorPos(pWindow, &x, &y);
		vCamRot.y -= fSens * (x - w * .5f);
		vCamRot.x -= fSens * (y - h * .5f);
		vCamRot.x = glm::clamp(vCamRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());

		glfwSetCursorPos(pWindow, w * .5f, h * .5f);

		rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);
	}

	if (type == "orbit") //rotates so that it always looks at its target, from a certain distance
	{
		glfwGetCursorPos(pWindow, &x, &y);

		vCamRot.y += fSens * (x - w * .5f);
		vCamRot.x += fSens * (y - h * .5f);
		vCamRot.x = glm::clamp(vCamRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());
		rotMat = (glm::mat3)glm::yawPitchRoll(vCamRot.y, vCamRot.x, vCamRot.z);
		glfwSetCursorPos(pWindow, w * .5f, h * .5f);
	}
}

void Camera::Movement()
{
		glm::vec3 dir(0, 0, 0);
		if (Input[GLFW_KEY_A]) { dir.x = -20; }
		if (Input[GLFW_KEY_D]) { dir.x = 20; }

		if (Input[GLFW_KEY_W]) { dir.z= -20; }
		if (Input[GLFW_KEY_S]) { dir.z = 20; }
		
		if (Input[GLFW_KEY_SPACE]) { dir.y = 20; }
		if (Input[GLFW_KEY_LEFT_SHIFT] || Input[GLFW_KEY_RIGHT_SHIFT] ) { dir.y = -20; }

		float speed = 1.0f;
		if (dir != glm::vec3())
		{
			dir = glm::normalize(dir);
		}
		vCamVel = rotMat * dir * speed;
		vCamLoc += fDelt * vCamVel * 6.f;
}

//Pass to shaders
void Camera::Upload()
{
	camMat = glm::perspective(fFovy, fAspect, zNear, zFar) * glm::lookAt(vCamLoc, vCamLoc - rotMat[2], rotMat[1]);
	glUniformMatrix4fv(4, 1, GL_FALSE, &camMat[0][0]);
	glUniform3fv(CAMERA_LOCATION, 1, &vCamLoc[0]);
}

Camera::~Camera()
{
}

void Camera::setLoc(glm::vec3 loc)
{
	vCamLoc = loc;
}

glm::vec3 Camera::getLoc()
{
	return vCamLoc;
}
