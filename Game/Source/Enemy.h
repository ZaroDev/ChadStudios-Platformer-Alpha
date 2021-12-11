#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "App.h"
#include "Physics.h"
#include "Point.h"
#include "Animation.h"


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
	int CheckDistanceToPhysBody(PhysBody* PhysPos)
	{
		b2Vec2 dist = PhysPos->body->GetPosition() - pbody->body->GetPosition();

		return (abs(dist.x) + abs(dist.y));
	}
	virtual void Update(){}

public:
	bool setPendingToDelete = false;
	bool birdDirection = true;
	int health;
	int lastTime = 0;
	int currentTime = 0;
	b2Vec2 agroSpeed;
	b2Vec2 calmSpeed;
	b2Vec2 currentSpeed;

	//navegation AI

	iPoint nextMovePos;

	iPoint currentMapTilePosition;
	iPoint lastMapTilePosition;
	bool direction;

	int posCheckTime;
	int checkTimer;
	int posCheckTimeAgro;
	int checkTimerAgro;
	int startPosMargin;


	bool agroTowardsPlayer;
	int maxDistanceAgroBase;
	int maxDistanceAgroActive;
	int maxDistanceAgro;
	bool inSpawnPos = true;
	Animation anim;
	PhysBody* pbody;
	iPoint pos;
	int h, w;
	int range;
};

#endif