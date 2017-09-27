#pragma once
#include "Texture.h"
#include <FreeImage.h>
#include <GL\glew.h>

class CubeMap :
	public Texture
{
public:
	char* textureLoc;
	CubeMap(char* Path);
	bool LoadTexture() override;
	void UseTexture() override;
	void UnloadTexture() override;
	virtual ~CubeMap();
};

