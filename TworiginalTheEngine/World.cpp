#include "World.h"

//Creates a scene boundary, for anything that would need it
World::World()
{
	WorldSize = Center = MinBoundaries = MaxBoundaries = glm::vec3();
}

World::World(glm::vec3 Center, glm::vec3 WorldSize)
{
	this->WorldSize = WorldSize;
	this->Center = Center;
	SetBounds();
}

void World::SetBounds()
{
	MinBoundaries = Center - (WorldSize / 2.f);
	MaxBoundaries = Center + (WorldSize / 2.f);
}

World::~World()
{
}
