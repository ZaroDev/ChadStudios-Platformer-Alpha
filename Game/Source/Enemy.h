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

struct SDL_Texture;


class Enemy
{
public:
	Enemy(){}

	iPoint GetPos() { return pos; }
	void SetPos(float x, float y)
	{
		pos.x = x;
		pos.y = y;
	}
	bool CheckIfHasTarget()
	{
		uint dist = Distance(app->player->pos.x, app->player->pos.y, pos.x, pos.y);
		return dist < range;
	}
	bool IsBetween(int value, int a, int b)
	{
		return (value >= a && value <= b);
	}
	void DrawPath(SDL_Texture* tex)
	{
		if (currentPath != nullptr)
		{
			for (uint i = 0; i < currentPath->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(currentPath->At(i)->x, currentPath->At(i)->y);
				app->render->DrawTexture(tex, pos.x, pos.y);
			}
		}
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
			activeDist = Distance(pos.x, pos.y, dest.x, dest.y);

			if (indexDist < activeDist) {
				pathIndex = i;
				break;
			}
		}
	}

	virtual void Update(float dt){}

public:
	bool setPendingToDelete = false;
	
	int health;
	
	Animation anim;
	PhysBody* pbody;
	iPoint pos;
	int h, w;
	int speed = 10;

	const DynArray<iPoint>* currentPath;
	int range;
	bool hasTarget = false;

	float pathUpdateTimer;
	float pathUpdateTime;

	iPoint activeNode;

	int pathIndex;

	bool facingLeft;
	
	enum TYPE
	{
		RAT = -1,
		EAGLE
	};
	TYPE type;
};

#endif