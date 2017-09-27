#include "Zone1.h"

Zone1::Zone1()
{
	bIsActiveScene = true;
}

Zone1::Zone1(GLFWwindow * window, int shad, int sky)
{
	pWindow = window;
	shaderLoc = shad;
	skyLoc = sky;
	bIsActiveScene = true;
}

void Zone1::PreGame()
{
	//Seed
	srand(glfwGetTime());
	
	//Set Scene
	SceneSpace = World(glm::vec3(0, 0, 0), glm::vec3(300, 300, 300));
	Lights.push_back(glm::vec3(0, 0, 0));
	glUniform3fv(LIGHT_LOCATION, 1, &Lights[0][0]);
	sky = Skybox();

	//Set Camera(s)

	//Set up Camera stats
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);
	float zoom = 1;
	float fovy = glm::pi<float>() * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	//Creating Three Cameras, an orbit camera for the ship, and two debug flying cameras
	Camera Orbit1(fovy, aspect, zNear, zFar, pWindow, "orbit");
	Cameras.push_back(Orbit1);
	Camera Fly1(fovy, aspect, zNear, zFar, pWindow, "fly");
	Cameras.push_back(Fly1);
	Camera Fly2(fovy, aspect, zNear, zFar, pWindow, "fly");
	Cameras.push_back(Fly2);
	nTotalCams = Cameras.size();
	nActCamInd = 0;

	//Set models
	asteroidModel.Buffer();
	playerModel.Buffer();
	zoneGateModel.Buffer();
	TextModel.Buffer();

	//Load in all the text boxes
	LoadTextBoxes();

	//Load all Textures
	for (int i = 0; i < Texts.size(); i++)
	{
		Texts[i].LoadTexture();
	}
	
	asteroidTexture.LoadTexture();
	playerTexture.LoadTexture();
	zoneTexture.LoadTexture();
	alt.LoadTexture();

	//Load all Objects
	playerObj = Player(playerTexture, playerModel, 1);
	zoneGateObj = Obj(&zoneGateModel, &zoneTexture, 1);
	Billboard = Obj(&TextModel, &Texts[0], 0);
	Billboard.transform.location = Cameras[0].getLoc();
	
	//Set up all the asteroids on the field
	SetField();

	//Modify certain locations for objects, change scales and rotations from the standards
	playerObj.transform.location = glm::vec3(SceneSpace.MaxBoundaries.x, SceneSpace.Center.y, SceneSpace.Center.z);
	playerObj.transform.size = playerModel.getSize();
	playerObj.transform.scale *= .25f;
	playerObj.transform.rotation = glm::vec3(0, glm::pi<float>() / 2.f, 0);
	zoneGateObj.transform.location = glm::vec3(SceneSpace.MinBoundaries.x, SceneSpace.Center.y, SceneSpace.Center.z);

	Cameras[2].setLoc(zoneGateObj.transform.location);
}

void Zone1::Update(std::map<int, bool> prevPut, std::map<int, bool> Input, float delt)
{
	fDelt = delt;

#pragma region Debug Cameras
	if (Input[GLFW_KEY_C] && !prevPut[GLFW_KEY_C]) { nActCamInd++; }
	if (nActCamInd >= nTotalCams) { nActCamInd = 0; }

	if (Cameras[nActCamInd].type == "fly")
	{
		Cameras[nActCamInd].Update(delt, Input);
	}	
#pragma endregion

#pragma region Game Loop
	if (Cameras[nActCamInd].type == "orbit" && bCut)
	{
		Cameras[nActCamInd].Update(delt, playerObj);

#pragma region Tutorial	
		if (bTutorial)
		{
			Billboard.transform.location = Cameras[nActCamInd].getLoc() + glm::vec3(0, 0, -1);

			if (Input[GLFW_KEY_ENTER] && !prevPut[GLFW_KEY_ENTER]) { nActiveCard++; }
			if (nActiveCard == Texts.size())
			{
				bTutorial = false;
			}
			if (nActiveCard == 6 && skyBoxBitTimer < 3.f)
			{
				bSkyBoxBit = true;
			}
			if (nActiveCard == 7)
			{
				bSkyBoxBit = false;
			}
			if (nActiveCard == 8)
			{
				bAltTex = true;
			}
			if (nActiveCard == 9)
			{
				bAltTex = false;
			}
		}
#pragma endregion

#pragma region Game Region
		else
		{
			playerObj.Update(Input, prevPut, delt);
			playerObj.transform.Update(delt);

			//BULLET COLLISIONS
			for (int i = 0; i < playerObj.Bullets.size(); i++)
			{
				playerObj.Bullets[i].transform.Update(delt);
				for (int j = 0; j < asteroidObjs.size(); j++)
				{
					if (playerObj.Bullets[i].Collision(asteroidObjs[j]))
					{
						Split(j, i);
						break;
					}
				}
			}
			//END BULLET COLLISION

			//ASTEROID UPDATES AND COLLISION
			if (asteroidObjs.size() != 0)
			{
				for (int i = 0; i < asteroidObjs.size(); i++)
				{
					asteroidObjs[i].transform.Update(delt);
					for (int j = 0; j < asteroidObjs.size(); j++)
					{
						if (i != j)
						{
							if (asteroidObjs[i].Collision(asteroidObjs[j]))
							{
								glm::vec3 temp = asteroidObjs[i].transform.velocity * asteroidObjs[i].transform.fMass;
								asteroidObjs[i].transform.velocity = asteroidObjs[j].transform.velocity * asteroidObjs[j].transform.fMass;
								asteroidObjs[j].transform.velocity = temp;
								asteroidObjs[i].transform.force += asteroidObjs[i].transform.velocity;
								asteroidObjs[j].transform.force += asteroidObjs[j].transform.velocity;
								break;
							}
						}
					}
				}
			}
			
			if (bEndLevel) //If player decides not to destroy all asteroids, trigger this instead
			{
				for (int i = 0; i < asteroidObjs.size(); i++)
				{
					if (asteroidObjs[i].transform.location.y < SceneSpace.MinBoundaries.y)
					{
						asteroidObjs.erase(asteroidObjs.begin() + i);
						asteroidObjs.shrink_to_fit();
						break;
					}
				}
			}
			else
			{
				BoundaryBox();
			}
			//END ASTEROID UPDATES

			//LEVEL TRANSITION CHECKS
			if (asteroidObjs.size() == 0)
			{
				if (glm::distance(playerObj.transform.location, zoneGateObj.transform.location) < 5 && bCut)
				{
					Cutscene();
					bCut = false;
					bIsActiveScene = false;
				}
			}
			//END LEVER TRANSITION CHECKS

			//LEVEL AUTO FINISH
			if (Input[GLFW_KEY_PAGE_DOWN])
			{
				bEndLevel = true;
				for (int i = 0; i < asteroidObjs.size(); i++)
				{
					asteroidObjs[i].transform.bGravity = true;
				}
			}
			//END LEVEL AUTO FINISH
		}
#pragma endregion
	}
#pragma endregion

}

void Zone1::Render()
{
	if (bTutorial)
	{
		glUseProgram(shaderLoc); 
		//Load the broken texture for second glitch
		if (nActiveCard == 8 && altTexBitTimer < 1.f)
		{
			altTexBitTimer += fDelt;
		}
		else
		{
			Billboard.texture = &Texts[nActiveCard];
			Billboard.texture->UseTexture();
			Billboard.Render();

			//This will play the skybox glitch event
			if (bSkyBoxBit) {
				//Skybox
				glUseProgram(skyLoc);
				glm::vec3 temp = Cameras[nActCamInd].getLoc();
				Cameras[nActCamInd].setLoc(glm::vec3());
				Cameras[nActCamInd].Upload();
				Cameras[nActCamInd].setLoc(temp);
				sky.Render();
				skyBoxBitTimer += fDelt;
				if (skyBoxBitTimer > .75f)
				{
					bSkyBoxBit = false;
				};
			}
		}
	}
	glUseProgram(shaderLoc);

	Cameras[nActCamInd].Upload();
	if (asteroidObjs.size() != 0)
	{
		asteroidObjs[0].texture->UseTexture();

		for (int i = 0; i < asteroidObjs.size(); i++)
		{
			asteroidObjs[i].Render();
		}
	}
	if (bAltTex)
	{
		playerObj.texture = &alt;
	}
	else
	{
		playerObj.texture = &playerTexture;
	}
	playerObj.texture->UseTexture();
	playerObj.Render();
	
	zoneGateObj.texture->UseTexture();
	zoneGateObj.Render();

	for (int i = 0; i < playerObj.Bullets.size(); i++)
	{
		playerObj.Bullets[i].Render();
	}

}

//This sets up the asteroid field for the zone
void Zone1::SetField()
{
	int asteroidCount = rand() % 50;
	asteroidCount += 15;
	for (int i = 0; i < asteroidCount; i++)
	{
		asteroidObjs.push_back(Obj(&asteroidModel, &asteroidTexture, 2));
		asteroidObjs[i].transform.bGravity = false;
		float x, y, z;

		//Generate a 3D point between bottoms and the tops of each axis
		x = rand() % (int)(SceneSpace.MaxBoundaries.x * 2) + (-SceneSpace.MaxBoundaries.x);
		y = rand() % (int)(SceneSpace.MaxBoundaries.y * 2) + (-SceneSpace.MaxBoundaries.y);
		z = rand() % (int)(SceneSpace.MaxBoundaries.z * 2) + (-SceneSpace.MaxBoundaries.z);

		asteroidObjs[i].transform.location = glm::vec3(x, y, z);

		x = rand() % 11 + (-5);
		y = rand() % 11 + (-5);
		z = rand() % 11 + (-5);

		asteroidObjs[i].transform.scale *= 4;
		asteroidObjs[i].transform.setSize(asteroidModel.getSize());
		asteroidObjs[i].transform.velocity = glm::vec3(x, y, z);
		
	}
}

//screen wrap the asteroids
void Zone1::BoundaryBox()
{
	for (int i = 0; i < asteroidObjs.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (asteroidObjs[i].transform.location[j] > SceneSpace.MaxBoundaries[j])
			{
				asteroidObjs[i].transform.location[j] = SceneSpace.MinBoundaries[j];
			}
			if (asteroidObjs[i].transform.location[j] < SceneSpace.MinBoundaries[j])
			{
				asteroidObjs[i].transform.location[j] = SceneSpace.MaxBoundaries[j];
			}
		}
	}
}

//Split up the asteroids into smaller bits when hit
void Zone1::Split(int index, int bul)
{
	glm::vec3 vel = asteroidObjs[index].transform.velocity;
	glm::vec3 pos = asteroidObjs[index].transform.location;
	glm::vec3 sca = asteroidObjs[index].transform.scale;
	asteroidObjs.erase(asteroidObjs.begin() + index);
	asteroidObjs.shrink_to_fit();
	
	if (sca.x > 1.f) //as long as the asteroid is not too small to be worth splitting
	{
		for (int i = 0; i < 8; i++) //create 8 smaller asteroids
		{
			Obj roid = Obj(&asteroidModel, &asteroidTexture, 2);
			roid.transform.location = pos;
			switch (i)
			{
			case 0: pos.y += roid.transform.size.y * 4; break;
			case 2: pos.y -= roid.transform.size.y * 4; break;
			case 3: pos.x += roid.transform.size.x * 4; break;
			case 5: pos.x -= roid.transform.size.x * 4; break;
			case 6: pos.z += roid.transform.size.z * 4; break;
			case 7: pos.z -= roid.transform.size.z * 4; break;
			}
			for (int j = 0; j < 3; j++)
			{
				vel[j] = vel[j] + (rand() % 5);
			}
			roid.transform.scale = sca / 2.f;
			roid.transform.fMass /= 8.f;
			roid.transform.setSize(roid.model->getSize());
			asteroidObjs.push_back(roid);
		}
	}
	playerObj.Bullets.erase(playerObj.Bullets.begin() + bul);
	playerObj.Bullets.shrink_to_fit();
}

//End of level transition (WIP)
void Zone1::Cutscene()
{
	playerObj.transform.location = zoneGateObj.transform.location + glm::vec3(4, 0, 0);
	playerObj.transform.LookAt(zoneGateObj.transform.location);
	float timer = 0.f;
	while (timer < 100.f)
	{
		timer += .001f;
	}
	for (int i = 0; i < 30; i++)
	{
		playerObj.transform.scale.z += .125f;
		playerObj.transform.scale.x -= .0025f;
		playerObj.transform.scale.z -= .0025f;
		zoneGateObj.texture->UseTexture();
		zoneGateObj.Render();
		playerObj.texture->UseTexture();
		playerObj.Render();
	}
	playerObj.transform.velocity += glm::vec3(15, 0, 0);
	bIsActiveScene = false;
}

void Zone1::LoadTextBoxes()
{
	Texts.push_back(Texture("textures\\Tutorial1.png"));
	Texts.push_back(Texture("textures\\Tutorial2.png"));
	Texts.push_back(Texture("textures\\Tutorial3.png"));
	Texts.push_back(Texture("textures\\Tutorial4.png"));
	Texts.push_back(Texture("textures\\Tutorial5.png"));
	Texts.push_back(Texture("textures\\Tutorial6.png"));
	Texts.push_back(Texture("textures\\Tutorial7.png"));
	Texts.push_back(Texture("textures\\Tutorial8.png"));
	Texts.push_back(Texture("textures\\Tutorial9.png"));
	Texts.push_back(Texture("textures\\Tutorial10.png"));
	Texts.push_back(Texture("textures\\Tutorial11.png"));
	Texts.push_back(Texture("textures\\Tutorial12.png"));


	//C++ apparently *does* care about unsafe. Can't append to char*s, cant copy strings to char*, gaah
	/*
	char* path = "textures\\Tutorial";
	char* ext = ".png";
	for (int i = 1; i < 13; i++)
	{
		char* num = (char*)i;
		char* Final = new char[std::strlen(path) + std::strlen(ext) + 2];
		std::strncpy(Final, path, std::strlen(path));
		std::strncat(Final, num, 1);
		std::strncat(Final, ext, std::strlen(ext));
		Texts.push_back(Texture(Final));
		delete[] Final;
	}
	*/
}


Zone1::~Zone1()
{
}
