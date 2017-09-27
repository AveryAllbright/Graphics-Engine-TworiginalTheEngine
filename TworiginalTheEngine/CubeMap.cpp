#include "CubeMap.h"

CubeMap::CubeMap(char* Path) : Texture(Path)
{
	textureLoc = Path;
}

//Rather than the usual texture, cubemaps need six textures (or 1 six times)
bool CubeMap::LoadTexture()
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	
	//Fix Seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Load and Bind each texture for each of the six faces
	for (int i = 0; i < 6; i++)
	{
		FIBITMAP* texture = FreeImage_Load(FreeImage_GetFileType(textureLoc), textureLoc);
		if (texture == nullptr) { return false; }
		image32Bit = FreeImage_ConvertTo32Bits(texture);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_SRGB_ALPHA,
			FreeImage_GetWidth(image32Bit),
			FreeImage_GetHeight(image32Bit),
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(image32Bit)
			);
		FreeImage_Unload(texture);
		FreeImage_Unload(image32Bit);
		
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}

//set the cubemap to the active texture in the shader
void CubeMap::UseTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
}

void CubeMap::UnloadTexture()
{
	glDeleteTextures(1, &texID);
}

CubeMap::~CubeMap()
{
}
