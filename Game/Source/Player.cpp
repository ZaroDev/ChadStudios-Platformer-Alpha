#include "Player.h"
#include "Physics.h"
#include "Input.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"
#include "Log.h"
#include "Window.h"


Player::Player(bool active)
{
	name.Create("player");
	idleAnimR.PushBack({ 8, 7, 24, 30 });
	idleAnimR.PushBack({ 51, 9, 25, 28 });
	idleAnimR.PushBack({ 98, 10, 28, 27 });
	idleAnimR.PushBack({ 139, 9, 25, 28 });
	idleAnimR.speed = 0.1f;
	idleAnimR.loop = true;

	idleAnimL.PushBack({ 314, 7, 24, 30 });
	idleAnimL.PushBack({ 270, 9, 25, 28 });
	idleAnimL.PushBack({ 226, 10, 28, 27 });
	idleAnimL.PushBack({ 189, 9, 25, 28 });
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


	grounded = true;
	/*b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y));
	body.fixedRotation = true;
	b2Body* b = app->physics->world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(24) * 0.5f, PIXEL_TO_METERS(20) * 0.5f);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 20.0f;
	fixture.friction = 100.0f;
	b->ResetMassData();
	b->CreateFixture(&fixture);*/

	b2BodyDef cbody;
	cbody.type = b2_dynamicBody;
	cbody.position.Set(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y));
	cbody.fixedRotation = true;
	b2Body* c = app->physics->world->CreateBody(&cbody);
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(12);
	b2FixtureDef fixturec;
	fixturec.shape = &circle;
	fixturec.density = 20.0f;
	fixturec.friction = 100.0f;
	c->ResetMassData();
	c->CreateFixture(&fixturec);


	


	pbody = new PhysBody();
	pbody->body = c;
	c->SetUserData(pbody);
	pbody->width = 24 * 0.5f;
	pbody->height = 27 * 0.5f;
	pbody->listener = this;

	debug = false;

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;


	grounded = false;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (pbody->body->GetLinearVelocity().y < 0.1f && pbody->body->GetLinearVelocity().y > -0.1f)
	{
		grounded = true;
	}
	if (!debug)
	{
		uint width, height;
		app->win->GetWindowSize(width, height);
		app->render->camera.x = -((pos.x * (int)app->win->GetScale()) - ((int)width) / 2 + pbody->width / 2);
		app->render->camera.y = -((pos.y * (int)app->win->GetScale()) - ((int)height) / 2 + pbody->height / 2);

		if (app->render->camera.x > 0)
			app->render->camera.x = 0;
		if (app->render->camera.y > 0)
			app->render->camera.y = 0;
		
	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		if (grounded)
			vel.x = 5.0f;
		else
			vel.x = 2.5f;
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
			vel.x = -5.0f;
		else
			vel.x = -2.5f;
		pbody->body->SetLinearVelocity(vel);
		if (currentAnimation != &runAnimL)
		{
			runAnimL.Reset();
			currentAnimation = &runAnimL;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && grounded)
	{
		pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, -5.0f });

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
	pbody->GetPosition(pos.x, pos.y);
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
