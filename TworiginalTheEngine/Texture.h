#pragma once
#include <FreeImage.h>
#include <GL/glew.h>
class Texture
{
protected:
	FIBITMAP* image32Bit;
	GLuint texID;
	char* texturePath;
public:
	Texture();
	Texture(char* texturePath);
	virtual bool LoadTexture();
	virtual void UseTexture();
	virtual void UnloadTexture();
	virtual ~Texture();
};

