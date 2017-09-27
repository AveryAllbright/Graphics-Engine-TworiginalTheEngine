#pragma once
#include "Scene.h"
#include <irrKlang.h>
class Zone1 :
	public Scene
{
public:
	Player playerObj;
	std::vector<Obj> asteroidObjs;
	Obj zoneGateObj;
	Obj Billboard;
	Model playerModel = Model("models\\ship.obj");
	Model asteroidModel = Model("models\\Rock.obj");
	Model zoneGateModel = Model("models\\ZoneGate.obj");
	Model TextModel = Model("models\\board.obj");

	std::vector<Texture> Texts;
	Texture playerTexture = Texture("textures\\playerTex.png");
	Texture alt = Texture("textures\\CartTexture.png");
	Texture asteroidTexture = Texture("textures\\rock.jpg");
	Texture zoneTexture = Texture("textures\\ZoneGate.png");

	bool bCut = true;
	bool bTutorial = true;
	bool bSkyBoxBit = false;
	bool bAltTex = false;
	bool bEndLevel = false;
	float skyBoxBitTimer = 0;
	float altTexBitTimer = 0;
	int nActiveCard = 0;

	Zone1();
	Zone1(GLFWwindow * window, int shad, int sky);
	void PreGame() override;
	void Update(std::map<int, bool> prevPut, std::map<int, bool>Input, float delt) override;
	void Render() override;
	void SetField();
	void BoundaryBox();
	void Split(int index, int bul);
	void Cutscene();
	void LoadTextBoxes();
	virtual ~Zone1();
};

