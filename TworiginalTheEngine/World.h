#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
class World
{
public:
	glm::vec3 WorldSize;
	glm::vec3 Center;
	glm::vec3 MinBoundaries;
	glm::vec3 MaxBoundaries;
	World();
	World(glm::vec3 Center, glm::vec3 WorldSize);
	void SetBounds();
	virtual ~World();
};

