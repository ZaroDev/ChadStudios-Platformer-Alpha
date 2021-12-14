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

class Player;


EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entities");
}

EntityManager::~EntityManager()
{
	CleanUp();
}

bool EntityManager::Awake(pugi::xml_node&)
{
	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		if (ent->data->setPendingToDelete) DestroyEntity(ent->data);
	}

	return true;
}
void EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (!doLogic)
		return;

	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		ent->data->Update(dt);
	}
	return;
}
bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;
	UpdateAll(dt, doLogic);
	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	return true;

}

bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		ent->data->Draw();
	}
	return true;
}

bool EntityManager::CleanUp()
{
	DestroyAllEntities();
	return true;
}

void EntityManager::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
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
		case EntityType::ENEMY_EAGLE: ret = new Eagle(position); break;
		case EntityType::ENEMY_RAT: ret = new Rat(position); break;
		case EntityType::GEM: ret = new Gem(position); break;
		case EntityType::CHERRY	: ret = new Cherry(position); break;
		case EntityType::CHECKPOINT: ret = new CheckPoint(position); break;
	}
	if (ret != nullptr) entities.Add(ret);
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	app->physics->world->DestroyBody(entity->pbody->body);
	entities.Del(entities.At(entities.Find(entity)));
}
void EntityManager::DestroyAllEntities()
{
	for (ListItem<Entity*>* ent = entities.start; ent != nullptr; ent = ent->next)
	{
		DestroyEntity(ent->data);
	}
}