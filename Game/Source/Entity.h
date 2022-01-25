#pragma once
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "App.h"
enum EntityType
{
	UNKNOWN = -1,
	PLAYER,
	ENEMY_EAGLE,
	ENEMY_RAT,
	GEM,
	CHERRY,
	CHECKPOINT,
	DOOR
};
enum EntityState
{
	NONE = -1,
	HURT
};

struct SDL_Texture;
class PhysBody;
class EntityManager;
class Entity
{
public:
	Entity() {}
	Entity(EntityType type_, iPoint position_) : type(type_), position(position_) {}

	virtual ~Entity() {};

	virtual void Update(float dt) {};
	virtual void Use() {};


	iPoint GetPos() { return position; }
	int GetHealth() { return health; }
	EntityState GetState() { return currentState; }
	void SetState(EntityState state) { currentState = state; }
	virtual bool LoadState(pugi::xml_node& data)	
	{
		bool ret = true;
		active = data.attribute("active").as_bool();

		return ret;
	}

	virtual bool SaveState(pugi::xml_node& data)
	{
		bool ret = true;
		
		data.append_attribute("active").set_value(active);

		return ret;
	}
	virtual void SetTarget(Entity* target) {};
	virtual Entity* GetTarget() { return nullptr; };
	friend class EntityManager;

protected:
	int ID;
	SString name;
	PhysBody* pbody;
	EntityType type;
	bool active = true;
	bool setPendingToDelete;
	iPoint position;
	Animation* currentAnimation;
	Animation anim;
	EntityState currentState = EntityState::NONE;
	int health;
	int h, w;
};