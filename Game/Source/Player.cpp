#include "Player.h"
#include "Physics.h"
#include "Input.h"
#include "Audio.h"
#include "App.h"
#include "Render.h"
#include "Map.h"
#include "UI.h"
#include "Textures.h"
#include "Log.h"
#include "EntityManager.h"

Player::Player(iPoint position_) : Entity(EntityType::PLAYER, position_)
{
	name.Create("player");
	LoadAnims();
	maxVel = 5;
	minVel = 2.5;
	jumpVel = -5;
	startPos = position_;
	currentAnimation = &idleAnimR;
	grounded = true;
	LOG("Player spawned at x%i, y%i", position.x, position.y);
	this->w = 20;
	this->h = 26;
	pbody = app->physics->CreateRectangle(position.x, position.y, w, h, DYNAMIC);
	pbody->body->SetFixedRotation(true);
	this->pbody->eListener = this;
	useDownDash = false;
	god = false;
	this->health = 3;
}
void Player::Use()
{

}

void Player::Update(float dt)
{
	bool ret = true;

	if (this->health <= 0 || this->position.y >= 1000)
	{
		app->die = true;
		return;
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pbody->body->GetPosition().y);

	grounded = false;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god = !god;
	if (app->input->GetKey(SDL_SCANCODE_F3))
	{
		pbody->body->SetTransform({PIXEL_TO_METERS(startPos.x), PIXEL_TO_METERS(startPos.y) }, 0.0f);
	}
	if (pbody->body->GetLinearVelocity().y < 0.1f && pbody->body->GetLinearVelocity().y > -0.1f)
		grounded = true;
			
	if (pbody->body->GetLinearVelocity().y == 0)
		numJumps = 2;
	
	


	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		if (grounded)
			vel.x = maxVel;
		else
			vel.x = minVel;
		pbody->body->SetLinearVelocity(vel);
		if(currentAnimation != &runAnimR )
		{
			runAnimR.Reset();
			currentAnimation = &runAnimR;
		} 
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		if (grounded)
			vel.x = -maxVel;
		else
			vel.x = -minVel;
		pbody->body->SetLinearVelocity(vel);
		if (currentAnimation != &runAnimL)
		{
			runAnimL.Reset();
			currentAnimation = &runAnimL;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && numJumps > 0)
	{
		app->audio->PlayFx(jumpSFX);
		pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, jumpVel });
		numJumps--;
		app->entman->PlayJump();
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && grounded)
	{
		counterDash++;
		if (counterDash == 60 && useDownDash == false)
		{
			pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, jumpVel * 1.5f });
			useDownDash = true;
			app->entman->PlaySuperJump();
		}
		if (useDownDash == false)
		{
			if (currentAnimation == &idleAnimL)
			{
				abilityAnimL.Reset();
				currentAnimation = &abilityAnimL;
			}
			if (currentAnimation == &idleAnimR)
			{
				abilityAnimR.Reset();
				currentAnimation = &abilityAnimR;
			}
		}
	}
	else
	{
		counterDash = 0;
	}
	if (useDownDash)
	{
		abilityCD++;
		if(abilityCD == 360)
		{
			abilityCD = 0;
			useDownDash = false;
		}
	}

	if (currentState == EntityState::HURT)
	{
		counter++;
		if (currentAnimation != &hurtAnim)
		{
			currentAnimation = &hurtAnim;
		}
		if (counter == 60)
		{
			currentState = NONE;
		}
	}
	if (currentState != EntityState::HURT)
	{
		counter = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE 
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		if (currentAnimation == &runAnimR || currentAnimation == &jumpAnimR || currentAnimation == &downAnimR
			|| currentAnimation == &abilityAnimR && hurt == false)
		{
			currentAnimation = &idleAnimR;
		}
		
		if (currentAnimation == &runAnimL || currentAnimation == &jumpAnimL || currentAnimation == &downAnimL 
			 || currentAnimation == &abilityAnimL && hurt == false)
		{
			currentAnimation = &idleAnimL;
		}
	}
	if (!grounded)
	{
		if (pbody->body->GetLinearVelocity().y < -1.0f && pbody->body->GetLinearVelocity().x > 0.1f)
			if (currentAnimation != &jumpAnimR)
				currentAnimation = &jumpAnimR;
		if(pbody->body->GetLinearVelocity().y < -1.0f && pbody->body->GetLinearVelocity().x < -0.1f)
			if (currentAnimation != &jumpAnimL)
				currentAnimation = &jumpAnimL;
		
		if (pbody->body->GetLinearVelocity().y > 0.1f && pbody->body->GetLinearVelocity().x < 0.1f)
			if (currentAnimation != &downAnimL)
				currentAnimation = &downAnimL;
		if (pbody->body->GetLinearVelocity().y > 0.1f && pbody->body->GetLinearVelocity().x > -0.1f)
			if (currentAnimation != &downAnimR)
				currentAnimation = &downAnimR;
	}

	
	
	currentAnimation->Update();

}

void Player::LoadAnims()
{
	idleAnimR.PushBack({ 8, 7, 24, 30 });
	idleAnimR.PushBack({ 51, 9, 25, 28 });
	idleAnimR.PushBack({ 92, 10, 28, 27 });
	idleAnimR.PushBack({ 139, 9, 25, 28 });
	idleAnimR.speed = 0.1f;
	idleAnimR.loop = true;

	idleAnimL.PushBack({ 314, 7, 24, 30 });
	idleAnimL.PushBack({ 270, 9, 25, 28 });
	idleAnimL.PushBack({ 226, 10, 28, 27 });
	idleAnimL.PushBack({ 182, 9, 25, 28 });
	idleAnimL.speed = 0.1f;
	idleAnimL.loop = true;

	runAnimR.PushBack({ 260, 89, 24, 30 });
	runAnimR.PushBack({ 303, 88, 26, 30 });
	runAnimR.PushBack({ 348, 89, 24, 29 });
	runAnimR.PushBack({ 393, 89, 24, 30 });
	runAnimR.PushBack({ 439, 88, 22, 30 });
	runAnimR.PushBack({ 481, 89, 24, 30 });
	runAnimR.speed = 0.1f;
	runAnimR.loop = true;

	runAnimL.PushBack({ 226, 89, 24, 30 });
	runAnimL.PushBack({ 181, 88, 26, 30 });
	runAnimL.PushBack({ 138, 89, 24, 29 });
	runAnimL.PushBack({ 93, 89, 24, 30 });
	runAnimL.PushBack({ 49, 88, 22, 30 });
	runAnimL.PushBack({ 5, 89, 24, 30 });
	runAnimL.speed = 0.1f;
	runAnimL.loop = true;

	jumpAnimR.PushBack({ 5, 50, 31, 28 });
	jumpAnimR.loop = false;
	downAnimR.PushBack({ 51, 47, 28, 30 });
	downAnimR.loop = false;
	jumpAnimL.PushBack({ 139, 49, 31, 28 });
	jumpAnimL.loop = false;
	downAnimL.PushBack({ 96, 46, 28, 30 });
	downAnimL.loop = false;

	hurtAnim.PushBack({ 5, 126, 30, 31 });
	hurtAnim.PushBack({ 46, 128, 29, 31 });
	hurtAnim.loop = true;
	hurtAnim.speed = 0.1f;

	abilityAnimR.PushBack({ 90, 129, 29, 27 });
	abilityAnimR.PushBack({ 138, 129, 28, 27 });
	abilityAnimR.loop = true;
	abilityAnimR.speed = 0.1f;

	abilityAnimL.PushBack({ 88, 157, 28, 27 });
	abilityAnimL.PushBack({ 135, 157, 29, 27 });
	abilityAnimL.loop = true;
	abilityAnimL.speed = 0.1f;
}


bool Player::LoadState(pugi::xml_node& data)
{
	bool ret = true;
	position.x = data.child("pos").attribute("x").as_int();
	position.y = data.child("pos").attribute("y").as_int();
	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
	health = data.child("values").attribute("lives").as_int();
	
	return ret;
}


bool Player::SaveState(pugi::xml_node&data) 
{
	bool ret = true;
	pugi::xml_node posN = data.append_child("pos");
	posN.append_attribute("x").set_value(position.x);
	posN.append_attribute("y").set_value(position.y);
	pugi::xml_node values = data.append_child("values");
	values.append_attribute("lives").set_value(health);
	return true;
}
