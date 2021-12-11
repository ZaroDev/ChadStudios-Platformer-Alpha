#include "Rat.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "SDL/include/SDL.h"

Rat::Rat() : Enemy()
{
	anim.PushBack({ 0,3, 28, 25 });
	anim.PushBack({ 39, 4, 29, 22 });
	anim.PushBack({ 75, 6, 32, 20 });
	anim.PushBack({ 111, 6, 35, 20 });
	anim.PushBack({ 147, 6, 35, 20 });
	anim.PushBack({ 183, 3, 31, 23 });
	anim.speed = 0.1f;
	anim.loop = true;
	h = 20;
	w = 20;
	health = 1;
	range = 5;
}

Rat::~Rat()
{
}


void Rat::Update(float dt)
{
	anim.Update();

	hasTarget = CheckIfHasTarget();

	//The enemy has only to move if it's in range of the player
	if (hasTarget && health > 0 && app->player->lives > 0)
	{
		ComputePath();
		MoveToPlayer();
	}
	else
	{
		pbody->body->SetLinearVelocity({ 0, 0 });
	
	}
	if (pbody->body->GetLinearVelocity().x <= 0.1)
	{
		facingLeft = true;
	}
	else
	{
		facingLeft = false;
	}
}

void Rat::ComputePath()
{
	iPoint origin = { (int)METERS_TO_PIXELS((int)pbody->body->GetPosition().x), (int)METERS_TO_PIXELS((int)pbody->body->GetPosition().y) };
	iPoint dest = { (int)METERS_TO_PIXELS((int)app->player->pbody->body->GetPosition().x + 24), (int)METERS_TO_PIXELS((int)app->player->pbody->body->GetPosition().y + 24) };

	origin = app->map->WorldToMap(origin.x, origin.y);
	dest = app->map->WorldToMap(dest.x, dest.y);

	app->pathfinding->CreatePath(origin, dest);
	currentPath = app->pathfinding->GetLastPath();
}

void Rat::MoveToPlayer()
{
	
}
