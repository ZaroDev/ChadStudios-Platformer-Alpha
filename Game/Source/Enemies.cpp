#include "Enemies.h"
#include "Eagle.h"
#include "Rat.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Player.h"
#include "Audio.h"

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
	sfx.Create(config.child("sfx").child_value());
	return true;
}

bool Enemies::Start()
{
	SString tmp("%s%s", folder.GetString(), "enemies.png");
	SString tmp2("%s%s", folder.GetString(), "nav.png");
	SString tmp3("%s%s", sfx.GetString(), "enemy.wav");
	SString tmp4("%s%s", sfx.GetString(), "playerHurt.wav");
	hitSFX = app->audio->LoadFx(tmp3.GetString());
	playerHit = app->audio->LoadFx(tmp4.GetString());
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
		e->data->Update(dt);
		if (e->data->facingLeft)
			app->render->DrawTexture(tex, x, y, &e->data->anim.GetCurrentFrame());
		else if (!e->data->facingLeft)
			app->render->DrawTexture(tex, x, y, &e->data->anim.GetCurrentFrame(), false, 1.0,  SDL_FLIP_HORIZONTAL);
	}
	return true;
}

bool Enemies::PostUpdate()
{
	if (app->player->debug)
	{
		for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
		{
			e->data->DrawPath(path);
		}
	}
	return true;
}

bool Enemies::DestroyEnemies()
{
	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		app->physics->world->DestroyBody(e->data->pbody->body);
	}
	enemies.Clear();
	return true;
}

void Enemies::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	for (ListItem<Enemy*>* e = enemies.start; e != NULL; e = e->next)
	{
		if (bodyA == e->data->pbody && bodyB->listener == (Module*)app->player)
		{
			
			float topA = bodyA->body->GetPosition().y - PIXEL_TO_METERS(e->data->h / 2);
			float botA = bodyA->body->GetPosition().y + PIXEL_TO_METERS(e->data->h / 2);
			float topB = bodyB->body->GetPosition().y + PIXEL_TO_METERS(12);
			float botB = bodyB->body->GetPosition().y - PIXEL_TO_METERS(12);

			if (topA >= botB)
			{
				e->data->health--;
				app->audio->PlayFx(hitSFX);
				if (e->data->health <= 0) e->data->setPendingToDelete = true;
			}

			else if (topA < botB && !app->player->hurt)
			{
				if (!app->player->god)
				{
					app->audio->PlayFx(playerHit);
					app->player->hurt = true;
				}
			}
			
		}
	}
}

void Enemies::CreateEnemy(EnemyType type, float x, float y, int health)
{
	switch (type)
	{
	case EAGLE:
	{
		Eagle* e = new Eagle();
		e->SetPos(x - 20, y - 20);
		e->pbody = app->physics->CreateRectangle(e->GetPos().x + e->w / 2, e->GetPos().y + e->h / 2, e->w, e->h, DYNAMIC);
		e->pbody->listener = this;
		e->pbody->body->SetFixedRotation(true);
		e->health = health;
		enemies.Add(e);
		break;
	}
	case RAT:
	{
		Rat* r = new Rat();
		r->SetPos(x - r->w / 2, y - r->h / 2);
		r->pbody = app->physics->CreateRectangle(r->GetPos().x + r->w / 2, r->GetPos().y + r->h / 2, r->w, r->h, DYNAMIC);
		r->pbody->listener = this;
		r->pbody->body->SetFixedRotation(false);
		r->health = health;
		enemies.Add(r);
		break;
	}
	case MAX:
		break;
	default:
		break;
	}
}


bool Enemies::SaveState(pugi::xml_node& entities_node) const
{
	// Clear previous saved entities
	pugi::xml_node entity_node = entities_node.first_child();

	while (entity_node) {
		entities_node.remove_child(entity_node);
		entity_node = entities_node.first_child();
	}

	// Add new entities
	ListItem<Enemy*>* entity;
	
	for (entity = enemies.start; entity != NULL; entity = entity->next) {
		Enemy* ent = entity->data;
		if (ent->health >= 1)
		{
			entity_node = entities_node.append_child("entity");

			entity_node.append_attribute("type") = ent->type;

			iPoint pos = ent->GetPos();
			entity_node.append_attribute("x") = pos.x;
			entity_node.append_attribute("y") = pos.y;
			entity_node.append_attribute("health") = ent->health;
		}

	}
	return true;
}

bool Enemies::CleanUp()
{
	DestroyEnemies();
	app->tex->UnLoad(tex);

	return true;
}

bool Enemies::LoadState(pugi::xml_node& entities_node)
{
	pugi::xml_node entity_node = entities_node.first_child();
	DestroyEnemies();
	while (entity_node) {
		EnemyType type = (EnemyType)entity_node.attribute("type").as_int();
		iPoint pos;
		pos.x = entity_node.attribute("x").as_int();
		pos.y = entity_node.attribute("y").as_int();

		int hp = entity_node.attribute("health").as_int();

		CreateEnemy(type, pos.x, pos.y, hp);

		entity_node = entity_node.next_sibling();
	}
	return true;
}