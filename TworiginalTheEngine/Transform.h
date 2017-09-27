#pragma once
#include<GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
class Transform
{
public:
	glm::vec3 location;
	glm::vec3 velocity;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 force;
	glm::vec3 size;
	glm::vec3 forward;
	glm::mat4 objWorldMatrix;
	float fMass;
	bool bJumped;
	bool bGravity;

	GLuint locl;
	Transform();
	void Update(float delt);
	void LookAt(glm::vec3 target);
	void Upload();
	virtual ~Transform();
	void setSize(glm::vec3 modSize);
};

