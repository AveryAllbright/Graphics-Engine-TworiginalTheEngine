#pragma once
#include "Obj.h"
#include "CubeMap.h"
class Skybox :
	public Obj
{
public:
	Skybox();
	void Render() override;
	virtual ~Skybox();
};

