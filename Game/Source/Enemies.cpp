#include "Enemies.h"
#include "Eagle.h"
#include "Rat.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
Enemies::Enemies(bool startEnabled) : Module(startEnabled)
{
	name.Create("enemies");
}

Enemies::~Enemies()
{
}

bool Enemies::Awake(pugi::xml_node&config)
{
	folder.Create(config.child("folder").child_value());
	
	return true;
}

bool Enemies::Start()
{
	SString tmp("%s%s", folder.GetString(), "enemies.png");
	SString tmp2("%s%s", folder.GetString(), "nav.png");
	tex = app->tex->Load(tmp.GetString());
	path = app->tex->Load(tmp2.GetString());
	return true;
}

bool Enemies::PreUpdate()
{
	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		if (e->data->setPendingToDelete)
		{
			app->physics->world->DestroyBody(e->data->pbody->body);
			enemies.Del(e);
			break;
		}
	}
	return true;
}

bool Enemies::Update(float dt)
{
	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		int x, y;
		e->data->pbody->GetPosition(x, y);
		e->data->Update();
		if (e->data->facingLeft)
			app->render->DrawTexture(tex, x, y, &e->data->anim.GetCurrentFrame());
		else if (!e->data->facingLeft)
			app->render->DrawTexture(tex, x, y, &e->data->anim.GetCurrentFrame(), false, 1.0,  SDL_FLIP_HORIZONTAL);
	}
	return true;
}

bool Enemies::PostUpdate()
{
	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		e->data->DrawPath(path);
	}
	return true;
}

bool Enemies::CleanUp()
{
	enemies.Clear();
	return true;
}

void Enemies::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		if (bodyA == e->data->pbody && bodyB->listener == (Module*)app->player)
		{
			if (bodyA->body->GetPosition().y >= bodyB->body->GetPosition().y)
			{
				e->data->health--;
				if (e->data->health <= 0) e->data->setPendingToDelete = true;
			}
			else if (bodyA->body->GetPosition().y < bodyB->body->GetPosition().y)
			{
				app->player->hurt = true;
			}
		}
	}
}

void Enemies::CreateEnemy(EnemyType type, float x, float y)
{
	switch (type)
	{
	case EAGLE:
	{
		Eagle* e = new Eagle();
		e->SetPos(x - 20, y - 20);
		e->pbody = app->physics->CreateRectangle(e->GetPos().x + e->w / 2, e->GetPos().y + e->h / 2, e->w, e->h, KINEMATIC);
		e->pbody->listener = this;
		enemies.Add(e);
		break;
	}
	case RAT:
	{
		Rat* r = new Rat();
		r->SetPos(x - r->w / 2, y - r->h / 2);
		r->pbody = app->physics->CreateRectangle(r->GetPos().x + r->w / 2, r->GetPos().y + r->h / 2, r->w, r->h, DYNAMIC);
		r->pbody->listener = this;
		enemies.Add(r);
		break;
	}
	case MAX:
		break;
	default:
		break;
	}
}


