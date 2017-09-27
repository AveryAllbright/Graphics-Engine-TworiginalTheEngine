#pragma once
#include "Obj.h"
class Bullet :
	public Obj
{
public:
	float burnTimer;
	Bullet();
	Bullet(Texture* tex, Model* mod, int col);
	virtual ~Bullet();
};

