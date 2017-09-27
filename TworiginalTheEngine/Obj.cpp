#include "Obj.h"

Obj::Obj()
{
}

Obj::Obj(Model * model, Texture * texture, int collider)
{
	this->model = model;
	this->texture = texture;
	this->collider = Colliders(collider);
	transform = Transform();
}

//The Render function uploads the current texture, the current model, and the transforms of that model
//to the shader to render
//It does not update any transformations
void Obj::Render()
{
	transform.Upload();
	model->Render();	
}

//Collision detection uses simple AABB and Sphere colliders
bool Obj::Collision(const Obj & obj)
{
	//No Collision can occur if either object has no collider
	if (collider == colliderless || obj.collider == colliderless)
	{
		return false;
	}

	//Sphere on Sphere simply checks if the distance from the centers are further than the radii of each object
	else if(collider == Sphere && obj.collider == Sphere)
	{
		float d = glm::distance(transform.location, obj.transform.location);
		if (d > (obj.transform.size.x / 2.0f) + (transform.size.x / 2.0f))
		{
			return false;
		}

	}
	//AABB checks against axis bound boxes. Since the model transform is in local space, and becomes world space, X Y Z should...
	//... be the world axis for each, meaning the box should resize if the model rotates far enough
	else if (collider == AABB && obj.collider == AABB)
	{
		float d = 0;
		for (int i = 0; i < 3; i++) 
		{
			d = glm::distance(transform.location[i], obj.transform.location[i]);
			if ( d > obj.transform.size[i] / 2.f + obj.transform.size[i] / 2.f) 
			{ return false; } 
		}
	}
	//AABB vs Sphere
	else
	{
		float s, d = 0;

		if (collider == Sphere && obj.collider == AABB) 
		{
			for (int i = 0; i < 3; i++)
			{
				if (transform.location[i] < obj.transform.location[i] - obj.transform.size[i] / 2.f)
				{
					s = transform.location[i] - (obj.transform.location[i] - obj.transform.size[i] / 2.f);
					d += s * s;
				}
				else if (transform.location[i] > obj.transform.location[i] + obj.transform.size[i] / 2.f)
				{
					s = transform.location[i] - (obj.transform.location[i] + obj.transform.size[i] / 2.f);
					d += s * s;
				}
			}
			if (d > transform.size.x /2.f ) { return false; }
		}
	}
	//All the above checks distance > size, meaning if the checks fall all the way through...
	//... to here, a collision has occurred.
	return true;
}

Obj::~Obj()
{
}

void Obj::SetCollider(int type)
{
	collider = Colliders(type);
}
