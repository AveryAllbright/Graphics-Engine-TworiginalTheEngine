#pragma once
#include "Model.h"
#include "Texture.h"
#include "Transform.h"
class Obj
{
public:
	enum Colliders
	{
		colliderless = 0,
		AABB = 1,
		Sphere = 2
	};

	bool bIsColliding = false;

	Colliders collider;
	Transform transform;
	Texture* texture;
	Model* model;
	Obj();
	Obj(Model* model, Texture* texture, int collider);
	virtual void Render();
	bool Collision(const Obj &obj);
	virtual ~Obj();
	
	void SetCollider(int type);
};

