#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "App.h"
#include "Physics.h"
#include "Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Map.h"
#include "Render.h"
#include "Entity.h"

struct SDL_Texture;


class Enemy : public Entity
{
public:
	Enemy(EntityType type, iPoint position_, Entity* target_) : Entity(type, position_), target(target_){}

	iPoint GetPos() { return position; }
	void SetPos(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	bool CheckIfHasTarget()
	{
		if (target == nullptr)
			return false;

		uint dist = Distance(target->GetPos().x, target->GetPos().y, position.x, position.y);
		return dist < range;

	}
	bool IsBetween(int value, int a, int b)
	{
		return (value >= a && value <= b);
	}

	int Distance(int x1, int y1, int x2, int y2)
	{
		return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
	}
	//Need to be improved Not used atm
	void ClosestPoint()
	{
		uint size = currentPath->Count();

		float indexDist;
		float activeDist;

		iPoint dest = app->map->MapToWorld(currentPath->At(size - 1)->x , currentPath->At(size - 1)->y);

		for (uint i = 0; i < size; i++) {
			iPoint indexO = app->map->MapToWorld(currentPath->At(i)->x, currentPath->At(i)->x);

			indexDist = Distance(indexO.x, indexO.y, dest.x, dest.y);
			activeDist = Distance(position.x, position.y, dest.x, dest.y);

			if (indexDist < activeDist) {
				pathIndex = i;
				break;
			}
		}
	}
	virtual void Update(float dt){}

	void SetTarget(Entity* target)
	{
		this->target = target;
	}
	Entity* GetTarget()
	{
		return target;
	}
	bool LoadState(pugi::xml_node& data)
	{
		bool ret = true;
		position.x = data.child("pos").attribute("x").as_int();
		position.y = data.child("pos").attribute("y").as_int();
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0.0f);
		health = data.child("values").attribute("lives").as_int();

		return ret;
	}


	bool SaveState(pugi::xml_node& data)
	{
		bool ret = true;
		pugi::xml_node posN = data.append_child("pos");
		posN.append_attribute("x").set_value(position.x);
		posN.append_attribute("y").set_value(position.y);
		pugi::xml_node values = data.append_child("values");
		values.append_attribute("lives").set_value(health);
		return true;
	}
public:
	int speed = 10;
	Entity* target;
	const DynArray<iPoint>* currentPath;
	int range;
	bool hasTarget = false;

	float pathUpdateTimer;
	float pathUpdateTime;

	iPoint activeNode;

	int pathIndex;

	bool facingLeft;
};

#endif