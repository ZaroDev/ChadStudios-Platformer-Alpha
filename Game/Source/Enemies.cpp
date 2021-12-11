#include "Enemies.h"
#include "Eagle.h"
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
	tex = app->tex->Load(folder.GetString());
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
		app->render->DrawTexture(tex, x, y, &e->data->anim.GetCurrentFrame());
		e->data->Update();
		
	}
	return true;
}

bool Enemies::PostUpdate()
{
	
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
		break;
	case MAX:
		break;
	default:
		break;
	}
}


