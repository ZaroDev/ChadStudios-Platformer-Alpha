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
	virtual void Update(){}
	//virtual void Awale(){}
public:
	SDL_Texture* tex;
	Animation anim;
private:
	iPoint pos;
	PhysBody* pbody;
	
};

#endif