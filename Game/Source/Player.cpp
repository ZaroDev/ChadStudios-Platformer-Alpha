#include "Player.h"
#include "Physics.h"
#include "Input.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"

Player::Player(bool active)
{
	idleAnimR.PushBack({ 8, 7, 24, 30 });
	idleAnimR.PushBack({ 51, 9, 25, 28 });
	idleAnimR.PushBack({ 98, 10, 28, 27 });
	idleAnimR.PushBack({ 139, 9, 25, 28 });
	idleAnimR.speed = 0.001f;
	idleAnimR.loop = true;
}

Player::~Player()
{
}

bool Player::Start()
{
	bool ret = true;
	tex = app->tex->Load("Assets/textures/sprites/player.png");
	currentAnimation = &idleAnimR;
	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;




	currentAnimation->Update();
	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(tex, pos.x, pos.y, &rect);
	return ret;
}

void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
}
