#include "Player.h"
#include "Physics.h"
#include "Input.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"
#include "Log.h"

Player::Player(bool active)
{
	name.Create("player");
	idleAnimR.PushBack({ 8, 7, 24, 30 });
	idleAnimR.PushBack({ 51, 9, 25, 28 });
	idleAnimR.PushBack({ 98, 10, 28, 27 });
	idleAnimR.PushBack({ 139, 9, 25, 28 });
	idleAnimR.speed = 0.001f;
	idleAnimR.loop = true;

	idleAnimL.PushBack({ 314, 7, 24, 30 });
	idleAnimL.PushBack({ 270, 9, 25, 28 });
	idleAnimL.PushBack({ 226, 10, 28, 27 });
	idleAnimL.PushBack({ 189, 9, 25, 28 });
	idleAnimL.speed = 0.001f;
	idleAnimL.loop = true;

	runAnimR.PushBack({ 260, 89, 24, 30 });
	runAnimR.PushBack({ 303, 88, 26, 30 });
	runAnimR.PushBack({ 348, 89, 24, 29 });
	runAnimR.PushBack({ 393, 89, 24, 30 });
	runAnimR.PushBack({ 439, 88, 22, 30 });
	runAnimR.PushBack({ 481, 89, 24, 30 });
	runAnimR.speed = 0.001f;
	runAnimR.loop = true;

	runAnimL.PushBack({ 226, 89, 24, 30 });
	runAnimL.PushBack({ 181, 88, 26, 30 });
	runAnimL.PushBack({ 138, 89, 24, 29 });
	runAnimL.PushBack({ 93, 89, 24, 30 });
	runAnimL.PushBack({ 49, 88, 22, 30 });
	runAnimL.PushBack({ 5, 89, 24, 30 });
	runAnimL.speed = 0.001f;
	runAnimL.loop = true;

}

Player::~Player()
{
}

bool Player::Awake(pugi::xml_node&config)
{
	LOG("Loading Player");
	bool ret = true;

	pos.x = config.child("pos").attribute("x").as_int();
	pos.y = config.child("pos").attribute("y").as_int();

	return ret;
}

bool Player::Start()
{
	bool ret = true;
	tex = app->tex->Load("Assets/textures/sprites/player.png");
	currentAnimation = &idleAnimR;
	body = app->physics->CreateRectangle(pos.x, pos.y, 24, 25, DYNAMIC);
	body->body->SetFixedRotation(true);
	grounded = true;
	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;
	b2Vec2 velocity = body->body->GetLinearVelocity();
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = body->body->GetLinearVelocity();
		vel.x = 0.1;
		body->body->SetLinearVelocity(vel);
		if(currentAnimation != &runAnimR && grounded)
		{
			runAnimR.Reset();
			currentAnimation = &runAnimR;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		b2Vec2 vel = body->body->GetLinearVelocity();
		vel.x = -0.1;
		body->body->SetLinearVelocity(vel);
		if (currentAnimation != &runAnimL && grounded)
		{
			runAnimL.Reset();
			currentAnimation = &runAnimL;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		body->body->SetLinearVelocity({ body->body->GetLinearVelocity().x, -0.5f});
		
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (currentAnimation == &runAnimR || currentAnimation == &jumpAnimR)
		{
			currentAnimation = &idleAnimR;
		}
		if (currentAnimation == &runAnimL || currentAnimation == &jumpAnimL)
		{
			currentAnimation = &idleAnimL;
		}
	}
	currentAnimation->Update();
	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	body->GetPosition(pos.x, pos.y);
	app->render->DrawTexture(tex, pos.x, pos.y, &rect);
	return ret;
}

void Player::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
}

bool Player::LoadState(pugi::xml_node&data)
{
	bool ret = true;
	pos.x = data.child("pos").attribute("x").as_int();
	pos.y = data.child("pos").attribute("y").as_int();
	return ret;
}

bool Player::SaveState(pugi::xml_node&data) const
{
	bool ret = true;
	pugi::xml_node posN = data.append_child("pos");
	posN.append_attribute("x").set_value(pos.x);
	posN.append_attribute("y").set_value(pos.y);

	return true;
}
