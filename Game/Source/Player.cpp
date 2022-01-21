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


Player::Player(iPoint position_) : Entity(EntityType::PLAYER, position_)
{
	maxVel = 5;
	minVel = 2.5;

	currentAnimation = &idleAnimR;
	grounded = true;
	position.x = 23;
	position.y = 800;
	LOG("Player spawned at x%f, y%f \n", position.x, position.y);
	this->w = 27;
	this->h = 27;
	this->pbody = app->physics->CreateRectangle(position.x, position.y, this->w, this->h, DYNAMIC);
	/*b2BodyDef cbody;
	cbody.type = b2_dynamicBody;
	cbody.position.Set(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
	cbody.fixedRotation = true;
	c = app->physics->world->CreateBody(&cbody);
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(12);
	b2FixtureDef fixturec;
	fixturec.shape = &circle;
	fixturec.density = 20.0f;
	fixturec.friction = 100.0f;
	c->ResetMassData();
	c->CreateFixture(&fixturec);

	this->pbody = new PhysBody();
	this->pbody->body = c;
	c->SetUserData(pbody);
	pbody->width = 24 * 0.5f;
	pbody->height = 27 * 0.5f;*/
	useDownDash = false;
	god = false;
	this->health = 3;
	//Initialize();
	LoadAnims();
}
void Player::Use()
{

}

void Player::Update(float dt)
{
	bool ret = true;


	grounded = false;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god = !god;
	if (app->input->GetKey(SDL_SCANCODE_F3))
	{
		if(currentScene == 1)
			pbody->body->SetTransform({ PIXEL_TO_METERS(scene1.x), PIXEL_TO_METERS(scene1.y) }, 0.0f);
		if(currentScene == 2)
			pbody->body->SetTransform({ PIXEL_TO_METERS(scene2.x), PIXEL_TO_METERS(scene2.y) }, 0.0f);
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
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && grounded)
	{
		counterDash++;
		if (counterDash == 60 && useDownDash == false)
		{
			pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, jumpVel * 1.5f });
			useDownDash = true;
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

	if (hurt)
	{
		counter++;
		if (currentAnimation != &hurtAnim)
		{
			currentAnimation = &hurtAnim;
		}
		if (counter == 60)
		{
			this->health--;
			hurt = false;
		}
	}
	if (!hurt)
	{
		counter = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		hurt = true;
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

	if (this->health <= 0)
	{
		app->die = true;
	}
	position.x = pbody->body->GetPosition().x;
	position.y = pbody->body->GetPosition().y;

	LOG("pos x%f y%f\n", position.x, position.y);
	LOG("pb x%f y%f\n", pbody->body->GetPosition().x, pbody->body->GetPosition().y);
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

void Player::Initialize()
{
	/*currentScene = config.child("start").attribute("value").as_int();
	numJumps = config.child("num_jumps").attribute("value").as_int();
	minVel = config.child("min_vel").attribute("value").as_float();
	maxVel = config.child("max_vel").attribute("value").as_float();
	jumpVel = config.child("jump_vel").attribute("value").as_float();
	folder.Create(config.child("folder").child_value());
	jumpSFXFile.Create(config.child("jump_SFX").child_value());
	scene1.x = config.child("scene1").attribute("x").as_int();
	scene1.y = config.child("scene1").attribute("y").as_int();

	scene2.x = config.child("scene2").attribute("x").as_int();
	scene2.y = config.child("scene2").attribute("y").as_int();*/

	maxVel = 5;
	minVel = 2.5;

	currentAnimation = &idleAnimR;
	grounded = true;
	position.x = 23;
	position.y = 800;
	b2BodyDef cbody;
	cbody.type = b2_dynamicBody;
	cbody.position.Set(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
	LOG("Player spawned at x%f, y%f \n", cbody.position.x, cbody.position.y);
	cbody.fixedRotation = true;
	c = app->physics->world->CreateBody(&cbody);
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(12);
	b2FixtureDef fixturec;
	fixturec.shape = &circle;
	fixturec.density = 20.0f;
	fixturec.friction = 100.0f;
	c->ResetMassData();
	c->CreateFixture(&fixturec);

	this->pbody = new PhysBody();
	pbody->body = c;
	c->SetUserData(pbody);
	pbody->width = 24 * 0.5f;
	pbody->height = 27 * 0.5f;
	useDownDash = false;
	god = false;
	this->w = 27;
	this->h = 27;
	this->health = 3;
}



/*bool Player::LoadState(pugi::xml_node& data)
{
	bool ret = true;
	pos.x = data.child("pos").attribute("x").as_int();
	pos.y = data.child("pos").attribute("y").as_int();
	pbody->body->SetTransform({ PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y) }, 0.0f);
	currentScene = data.child("start").attribute("value").as_int();
	lives = data.child("values").attribute("lives").as_int();
	score = data.child("values").attribute("score").as_int();
	return ret;
}


bool Player::SaveState(pugi::xml_node&data) const
{
	bool ret = true;
	pugi::xml_node posN = data.append_child("pos");
	posN.append_attribute("x").set_value(position.x);
	posN.append_attribute("y").set_value(pos.y);
	pugi::xml_node currentState = data.append_child("start");
	currentState.append_attribute("value").set_value(currentScene);
	pugi::xml_node values = data.append_child("values");
	values.append_attribute("lives").set_value(lives);
	values.append_attribute("score").set_value(score);
	return true;
}*/
