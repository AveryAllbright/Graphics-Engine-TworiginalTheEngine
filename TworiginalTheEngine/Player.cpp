#include "Player.h"

Player::Player()
{
}

Player::Player(Texture & texture, Model & model, int collider) : Obj(&model, &texture, collider)
{
	bulletModel.Buffer();
	bulletTex.LoadTexture();
	soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->setSoundVolume(.001f);

}

void Player::Update(std::map<int, bool> Input, std::map<int, bool> Prev, float delt)
{

#pragma region Input Updates
	// Forward Thrust
	if (Input[GLFW_KEY_W]) { transform.velocity = transform.forward * 15.f; soundEngine->play2D("sounds\\engine.wav"); }
	// Reverse Thrust
	if (Input[GLFW_KEY_S]) { transform.velocity = transform.forward * -7.5f; soundEngine->play2D("sounds\\engine.wav"); }
	//Stop Motion
	if (!Input[GLFW_KEY_S] && !Input[GLFW_KEY_W]) { transform.force = -transform.velocity; }
	// Up Pitch
	if (Input[GLFW_KEY_SPACE]) { transform.rotation += glm::vec3(.015f, 0, 0); }
	// Down Pitch
	if (Input[GLFW_KEY_LEFT_SHIFT] || Input[GLFW_KEY_RIGHT_SHIFT]) { transform.rotation -= glm::vec3(.015f,0, 0); }
	// Turn Point Left Yaw
	if (Input[GLFW_KEY_A]) {transform.rotation += glm::vec3(0, .015, 0); }
	// Turn Point Right Yaw
	if (Input[GLFW_KEY_D]) { transform.rotation -= glm::vec3( 0, .015f,  0); }
	// CoG Left Roll
	if (Input[GLFW_KEY_Q] && !bIsRolling) {  bIsRolling = 1; }
	//  CoG Right Roll
	if (Input[GLFW_KEY_E] && !bIsRolling) { bIsRolling = 2; }
	// Fire Cannons
	if (Input[GLFW_KEY_F] && !bIsFiring && FireTimer == 0) { bIsFiring = true; FireMainCannons(); }
	// Forward Thrusting Loop
	if (Input[GLFW_KEY_T] && !bIsLooping) {  bIsLooping = true; }
#pragma endregion

#pragma region Long Term Updates

	if (bIsRolling != 0)
	{
		Roll(bIsRolling);
		RollTimer += delt;
		if (RollTimer > 10.f)
		{
			bIsRolling = 0;
			RollTimer = 0;
		}
	}

	if (bIsFiring)
	{
		FireTimer += delt;
		if (FireTimer > .25f)
		{
			bIsFiring = false;
			FireTimer = 0.f;
		}
	}

	if (bIsLooping)
	{
		Loop();
		LoopTimer += delt;
		if (LoopTimer > 5.f)
		{
			bIsLooping = false;
			LoopTimer = 0;
		}
	}
#pragma endregion

#pragma region Bullet Updates
	if (Bullets.size() != 0)
	{
		for (int i = 0; i < Bullets.size(); i++)
		{
			Bullets[i].transform.Update(delt);
			Bullets[i].burnTimer += delt;
			if (Bullets[i].burnTimer > maxBurnTime)
			{
				Bullets.erase(Bullets.begin() + i);
				break;
			}
		}
	}
#pragma endregion

}

//Creates four new laser bullets in a small spread, going forward relative to the ship
void Player::FireMainCannons()
{
	
	for (int i = 0; i < 4; i++)
	{
		Bullet bullet = Bullet(&bulletTex, &bulletModel, 2);
		glm::vec3 vec = (transform.forward * 8.f) + transform.velocity;
		glm::vec3 pos = transform.location + (transform.forward * 1.5f) + glm::vec3((rand() % 101 + (-50)) / 25.f, (rand() % 101 + (-50)) / 25.f, (rand() % 101 + (-50)) / 25.f);
		bullet.transform.location = pos;
		bullet.transform.velocity = vec;
		bullet.transform.scale = glm::vec3(.15, .15, .75);
		bullet.transform.rotation = transform.rotation;
		Bullets.push_back(bullet);

		soundEngine->play2D("sounds\\fire.wav", false);
	}
}

//TODO : Implement forward loop
void Player::Loop()
{
}

//TODO : Implement aileron roll 
void Player::Roll(int direction)
{
}

Player::~Player()
{
	//nothing to do here
}
