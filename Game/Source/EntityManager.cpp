#include "EntityManager.h"
#include "Physics.h"
#include "App.h"
#include "CheckPoint.h"
#include "Rat.h"
#include "Player.h"
#include "CheckPoint.h"
#include "Eagle.h"
#include "Cherry.h"
#include "Gem.h"
#include "Door.h"
#include "Audio.h"
#include "Window.h"
#include "Render.h"
#include "Log.h"
#include "UI.h"
#include "Textures.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entities");
}

EntityManager::~EntityManager()
{
	CleanUp();
}

bool EntityManager::Awake(pugi::xml_node& config)
{
	folder.Create(config.child("folder").child_value());


	return true;
}

bool EntityManager::Start()
{
	SString pathPlayer("%s%s", folder.GetString(), "player.png");
	SString pathEnemies("%s%s", folder.GetString(), "enemies.png");
	SString pathCollect("%s%s", folder.GetString(), "collect.png");
	SString pathCheck("%s%s", folder.GetString(), "checkpoint.png");
	SString pathPath("%s%s", folder.GetString(), "nav.png");
	playerTex = app->tex->Load(pathPlayer.GetString());
	enemiesTex = app->tex->Load(pathEnemies.GetString());
	checkTex = app->tex->Load(pathCheck.GetString());
	collectTex = app->tex->Load(pathCollect.GetString());
	path = app->tex->Load(pathPath.GetString());

	return true;
}

bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		if (ent->data->setPendingToDelete)
		{
			DestroyEntity(ent->data);
			break;
		}
		if ((ent->data->type == EntityType::ENEMY_EAGLE || ent->data->type == EntityType::ENEMY_RAT) && ent->data->GetTarget() == nullptr)
		{
			ent->data->SetTarget(currentPlayer);
		}
	}
	OPTICK_CATEGORY("PreUpdate EntityManager", Optick::Category::AI);
	return true;
}
void EntityManager::UpdateAll(float dt)
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		ent->data->Update(dt);
	}
	return;
}
bool EntityManager::Update(float dt)
{
	UpdateAll(dt);

	if (currentPlayer != nullptr)
	{
		if (!currentPlayer->god)
		{
			//Camera follows the player
			uint width, height;

			app->win->GetWindowSize(width, height);
			app->render->camera.x = -((currentPlayer->GetPos().x * (int)app->win->GetScale()) - ((int)width) / 2 + currentPlayer->pbody->width / 2);
			app->render->camera.y = -((currentPlayer->GetPos().y * (int)app->win->GetScale()) - ((int)height) / 2 + currentPlayer->pbody->height / 2);
			//Camera bounds
			if (app->render->camera.x > 0)
				app->render->camera.x = 0;
			if (app->render->camera.y > 0)
				app->render->camera.y = 0;
			if (-app->render->camera.x > app->map->bounds.x)
				app->render->camera.x = -app->map->bounds.x;
			if (-app->render->camera.y > app->map->bounds.y)
				app->render->camera.y = -app->map->bounds.y;
		}
	}
	OPTICK_CATEGORY("Update EntityManager", Optick::Category::AI);

	return true;
}

bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		switch (ent->data->type)
		{
		case EntityType::PLAYER: app->render->DrawTexture(playerTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::ENEMY_EAGLE:app->render->DrawTexture(enemiesTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::ENEMY_RAT: app->render->DrawTexture(enemiesTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::GEM: app->render->DrawTexture(collectTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::CHERRY: app->render->DrawTexture(collectTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		case EntityType::CHECKPOINT: app->render->DrawTexture(checkTex, ent->data->GetPos().x - (ent->data->w / 2), ent->data->GetPos().y - (ent->data->h / 2), &ent->data->currentAnimation->GetCurrentFrame()); break;
		}
	}
	if (app->debug)
	{
		DrawPath(path);
	}
	OPTICK_CATEGORY("PostUpdate EntityManager", Optick::Category::AI);

	return true;
}

bool EntityManager::CleanUp()
{
	DestroyAllEntities();
	return true;
}

void EntityManager::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->eListener->type == PLAYER && bodyB->listener == this && bodyA->listener == this)
	{
		if (bodyB->eListener->type == ENEMY_EAGLE || bodyB->eListener->type == ENEMY_RAT)
		{

			float topA = bodyA->body->GetPosition().y - PIXEL_TO_METERS(bodyA->eListener->h / 2);
			float botA = bodyA->body->GetPosition().y + PIXEL_TO_METERS(bodyA->eListener->h / 2);
			float topB = bodyB->body->GetPosition().y + PIXEL_TO_METERS(bodyB->eListener->h / 2);
			float botB = bodyB->body->GetPosition().y - PIXEL_TO_METERS(bodyB->eListener->h / 2);

			if (topA <= botB)
			{
				bodyB->eListener->health--;
				app->audio->PlayFx(hitSFX);
				app->ui->AddScore(150);
			}

			else if (topA > botB && currentPlayer->GetState() != EntityState::HURT)
			{
				if (!currentPlayer->god)
				{
					bodyA->eListener->health--;
					app->audio->PlayFx(playerHit);
					currentPlayer->SetState(EntityState::HURT);
				}
			}
			return;
		}
		else if (bodyB->eListener->type == CHERRY)
		{
			if (bodyA->eListener->health < 3)
			{
				bodyA->eListener->health++;	
			}
			else
			{
				app->ui->AddScore(50);
			}
			bodyB->eListener->Use();
			return;
		}
		else if (bodyB->eListener->type == GEM)
		{
			bodyB->eListener->Use();
			app->ui->AddScore(100);
			return;
		}
		else if (bodyB->eListener->type == CHECKPOINT)
		{
			app->SaveGameRequest();
			bodyB->eListener->Use();
			return;
		}
	}
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	return false;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	return false;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint position)
{
	Entity* ret = nullptr;
	switch (type)
	{
		case EntityType::PLAYER: ret = new Player(position); break;
		case EntityType::ENEMY_EAGLE: ret = new Eagle(position, currentPlayer); break;
		case EntityType::ENEMY_RAT: ret = new Rat(position, currentPlayer); break;
		case EntityType::GEM: ret = new Gem(position); break;
		case EntityType::CHERRY	: ret = new Cherry(position); break;
		case EntityType::CHECKPOINT: ret = new CheckPoint(position); break;
		case EntityType::DOOR: ret = new Door(position); break;
	}

	if (ret != nullptr)
	{
		ret->pbody->listener = this;
		entities.Add(ret);
	}
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	app->physics->world->DestroyBody(entity->pbody->body);
	entities.Del(entities.At(entities.Find(entity)));
}
void EntityManager::SetPlayer(Player* player)
{
	currentPlayer = (Player*)player;
}
void EntityManager::DestroyAllEntities()
{
	entities.Clear();
}
void EntityManager::DrawPath(SDL_Texture* tex)
{
	const DynArray<iPoint>* currentPath = app->pathfinding->GetLastPath();
	if (currentPath != nullptr)
	{
		for (uint i = 0; i < currentPath->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(currentPath->At(i)->x, currentPath->At(i)->y);
			app->render->DrawTexture(tex, pos.x, pos.y);
		}
	}
}