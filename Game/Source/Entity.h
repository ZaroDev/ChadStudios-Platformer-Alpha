#pragma once
#include "Point.h"
#include "Animation.h"

enum EntityType
{
	UNKNOWN = -1,
	PLAYER,
	ENEMY_EAGLE,
	ENEMY_RAT,
	GEM,
	CHERRY,
	CHECKPOINT
};
struct SDL_Texture;
class PhysBody;
class EntityManager;
class Entity
{
public:
	Entity(){}
	Entity(EntityType type_, iPoint position_) : type(type_), position(position_){}

	virtual void Update(float dt){}
	virtual void Draw(){}

	friend class EntityManager;
protected:
	EntityType type;
	bool setPendingToDelete;
	iPoint position;
	SDL_Texture* sprite;
	PhysBody* pbody;
	Animation anim;
	int health;
};