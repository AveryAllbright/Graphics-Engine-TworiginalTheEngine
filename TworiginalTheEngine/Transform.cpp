#include "Transform.h"

//Shader location to push transform to
const GLuint MODEL_WORLD_LOCATION = 3;

//I have a real purpose this time
Transform::Transform()
{
	location = velocity = rotation = force = forward = glm::vec3();
	scale = size = glm::vec3(1, 1, 1);
	fMass = 1.f;
	bJumped = bGravity = false;
}

//Pushes movement. This mostly just deals with gravity as actual movement is controlled by the Objs
void Transform::Update(float delt)
{
	float fDrag = -.5f;
	if (bGravity)
	{
		force += fDrag * velocity;
		glm::vec3 Gravity = glm::vec3(0, -9.81, 0);
		force += fMass * Gravity;
	}
	
	velocity += (force / fMass) * delt;
	location += velocity * delt;

	for (int i = 0; i < 3; i++) //For that nice, smooth deceleration 
	{
		if (force[i] > 0)
		{
			force[i] += fDrag;
		}
		else if (force[i] < 0)
		{
			force[i] -= fDrag;
		}
	}
}

//forces an object to rotate such that its forward vector is directed at another object
void Transform::LookAt(glm::vec3 target)
{
	glm::vec3 Z = target - location;
	Z = glm::normalize(Z);
	glm::vec3 X = glm::normalize(glm::cross(glm::vec3(objWorldMatrix[1]), Z));
	glm::vec3 Y = glm::cross(Z, X);
	glm::mat4 Rot = {
		X.x, Y.x, Z.x, 0,
		X.y, Y.y, Z.y, 0,
		X.z, Y.z, Z.z, 0,
		0, 0, 0, 1
	};

	glm::mat4 loc = glm::translate(location);
	glm::mat4 sca = glm::scale(scale);
	objWorldMatrix = loc * Rot * sca;
	glUniformMatrix4fv(MODEL_WORLD_LOCATION, 1, GL_FALSE, &objWorldMatrix[0][0]);
}

//Push the transform to the shader
void Transform::Upload()
{
	glm::mat4 lock = glm::translate(location);
	glm::mat4 shock = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 barrel = glm::scale(scale);
	objWorldMatrix = lock * shock * barrel;

	glUniformMatrix4fv(MODEL_WORLD_LOCATION, 1, GL_FALSE, &objWorldMatrix[0][0]);

	forward = (glm::mat3)(shock)* glm::vec3(0, 0, -1);
}

Transform::~Transform()
{
	//Goodbye!
}

void Transform::setSize(glm::vec3 modSize)
{
	size = modSize * scale;
}
