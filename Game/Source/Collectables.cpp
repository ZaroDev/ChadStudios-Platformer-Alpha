#include "Collectables.h"
#include "Player.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"

void Collectables::CreateObj(Type type_, float x, float y)
{
	Collectable* c = new Collectable();
	c->type = type_;
	c->pos.x = x - 8;
	c->pos.y = y - 8;
	c->body = app->physics->CreateRectangleSensor(x, y, 16, 16, STATIC);
	c->body->listener = this;
	if (type_ == GEM)
	{
		c->anim.PushBack({ 0, 1, 13, 11 });
		c->anim.PushBack({ 15, 1, 13, 11 });
		c->anim.PushBack({ 30, 1, 13, 11 });
		c->anim.PushBack({ 45, 1, 13, 11 });
		c->anim.PushBack({ 60, 1, 13, 11 });
		c->anim.loop = true;
		c->anim.speed = 0.1f;
	}
	if (type_ == CHERRY)
	{
		c->anim.PushBack({ 0, 14, 16, 16 });
		c->anim.PushBack({ 22, 14, 16, 16 });
		c->anim.PushBack({ 44, 14, 16, 16 });
		c->anim.PushBack({ 67, 14, 16, 16 });
		c->anim.loop = true;
		c->anim.speed = 0.1f;
	}

	colectables.Add(c);
}

Collectables::Collectables(bool startEnabled) : Module(startEnabled)
{
	name.Create("collect");
}

Collectables::~Collectables()
{
}

bool Collectables::Awake(pugi::xml_node& config)
{

	folder.Create(config.child("folder").child_value());
	sfx.Create(config.child("sfx").child_value());
	return true;
}

bool Collectables::Start()
{
	tex = app->tex->Load(folder.GetString());
	SString tmp("%s%s", sfx.GetString(), "gem.wav");
	SString tmp2("%s%s", sfx.GetString(), "cherry.wav");
	gemSFX = app->audio->LoadFx(tmp.GetString());
	cherrySFX = app->audio->LoadFx(tmp2.GetString());
	return true;
}

bool Collectables::PreUpdate()
{
	for (ListItem<Collectable*>* c = colectables.start; c != NULL; c = c->next)
	{
		if (c->data->pendingToDelete)
		{
			app->physics->world->DestroyBody(c->data->body->body);
			colectables.Del(c);
			break;
		}
	}
	return true;
}

bool Collectables::Update(float dt)
{
	for (ListItem<Collectable*>* c = colectables.start; c != NULL; c = c->next)
	{
		c->data->anim.Update();
		app->render->DrawTexture(tex, c->data->pos.x, c->data->pos.y, &c->data->anim.GetCurrentFrame());
	}
	return true;
}


bool Collectables::CleanUp()
{
	for (ListItem<Collectable*>* c = colectables.start; c != NULL; c = c->next)
	{
		app->physics->world->DestroyBody(c->data->body->body);
	}
	app->tex->UnLoad(tex);
	colectables.Clear();
	return true;
}

void Collectables::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	for (ListItem<Collectable*>* c = colectables.start; c != NULL; c = c->next)
	{
		if (bodyA == c->data->body && bodyB->listener == (Module*)app->player)
		{
			if (c->data->type == CHERRY)
			{
				if (app->player->lives < 3)
				{
					app->player->lives++;
				}
				else
				{
					app->player->score += 50;
				}
				app->audio->PlayFx(cherrySFX);
			}
			if (c->data->type == GEM)
			{
				app->player->score += 10;
				app->audio->PlayFx(gemSFX);
			}
			c->data->pendingToDelete = true;
		}
		

	}
}
