#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "App.h"
#include "Physics.h"
#include "Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"

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
		uint dist = abs(sqrt(pow( pbody->body->GetPosition().x - app->player->pbody->body->GetPosition().x, 2) + pow(pbody->body->GetPosition().y - app->player->pbody->body->GetPosition().y, 2) ));
		return dist < range;
	}
	bool IsBetween(int value, int a, int b)
	{
		return (value >= a && value <= b);
	}



	virtual void Update(){}

public:
	bool setPendingToDelete = false;
	
	int health;
	
	Animation anim;
	PhysBody* pbody;
	iPoint pos;
	int h, w;
	b2Vec2 speed = { 1.0f, -1.0f };

	const DynArray<iPoint>* currentPath;
	int range;
	bool hasTarget = false;

};

#endif