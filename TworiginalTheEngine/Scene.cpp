#include "Scene.h"

// A purely abstract base class for use as a parent

Scene::Scene()
{

}

void Scene::PreGame()
{

}

void Scene::Update(std::map<int, bool> prevPut, std::map<int, bool> Input, float delt)
{

}

void Scene::Render()
{
}

Scene::~Scene()
{
}
