#include "Texture.h"

Texture::Texture()
{
	//I'm a safety measure!
}

//Path to texture location
Texture::Texture(char * texturePath) : texturePath(texturePath)
{
	
}

//Convert an image file into a FIBITMAP texture file for use in the shader
bool Texture::LoadTexture()
{
	FIBITMAP* texture = FreeImage_Load(FreeImage_GetFileType(texturePath), texturePath);
	if (texture == nullptr) { return false; }

	image32Bit = FreeImage_ConvertTo32Bits(texture);
	FreeImage_Unload(texture);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return true;
}

//set the active texture to this texture
void Texture::UseTexture()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

//Make sure to delete me before you leave
void Texture::UnloadTexture()
{
	FreeImage_Unload(image32Bit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	
}
