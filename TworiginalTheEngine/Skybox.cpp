#include "Skybox.h"

Skybox::Skybox()
{
	model = new Model("models\\box.obj");
	model->Buffer();

	texture = new CubeMap("textures\\Sky.jpg");
	texture->LoadTexture();
}

//Skybox rendering requires disabling cullfacing and depth masks, so you can see the inside of the map
void Skybox::Render()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	texture->UseTexture();
	model->Render();
	glUniform3fv(0, 1, &transform.location[0]);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);



}


Skybox::~Skybox()
{
}
