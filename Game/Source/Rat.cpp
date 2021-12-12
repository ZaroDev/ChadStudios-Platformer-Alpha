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
	
	range = 500;
	pathUpdateTime = 1.5f;
	pathUpdateTimer = pathUpdateTime;
}

Rat::~Rat()
{
}


void Rat::Update(float dt)
{
	anim.Update();

	hasTarget = CheckIfHasTarget();
	pos.x = METERS_TO_PIXELS(pbody->body->GetPosition().x);
	pos.y = METERS_TO_PIXELS(pbody->body->GetPosition().y);
	//The enemy has only to move if it's in range of the player
	if (hasTarget && health > 0 && app->player->lives > 0)
	{
		ComputePath(dt);
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

void Rat::ComputePath(float dt)
{
	iPoint playerPos = app->player->pos;
	float dist = Distance(pos.x, pos.y, playerPos.x, playerPos.y);

	pathUpdateTimer += dt;
	if (dist > range) {
		return;
	}
	else
	{
		if (pathUpdateTimer >= pathUpdateTime) {
			pathUpdateTimer = 0.0f;
			pathIndex = 0;

			iPoint origin = app->map->WorldToMap(pos.x, pos.y);
			iPoint destination = app->map->WorldToMap(app->player->pos.x, app->player->pos.y);
			int res = app->pathfinding->CreatePath(origin, destination);

			if (res > 0) {
				currentPath = app->pathfinding->GetLastPath();
				if (currentPath->Count() > 1) {
					pathIndex = 1;
					activeNode = app->map->MapToWorld(currentPath->At(pathIndex)->x, currentPath->At(pathIndex)->y);
				}
				else if (currentPath->Count() > 0)
				{
					activeNode = app->map->MapToWorld(currentPath->At(0)->x, currentPath->At(0)->y);
				}
			}
		}

		if (currentPath->Count() > 0) {
			if (pos == activeNode) {
				pathIndex++;

				if (pathIndex < currentPath->Count()) {
					activeNode = app->map->MapToWorld(currentPath->At(pathIndex)->x, currentPath->At(pathIndex)->y);
				}
			}

			if (pathIndex < currentPath->Count()) {
				MoveToPlayer(activeNode, dt);
			}
		}
	}
	if (!canJump)
	{
		counterJump++;
		if (counterJump >= 60)
			canJump = true;
	}
}

void Rat::MoveToPlayer(iPoint destination,float dt)
{
	iPoint diff = destination - pos;

	fPoint dir = { (float)diff.x, (float)diff.y };
	dir.Normalize();
	dir *= speed * 4;

	fPoint step = { dir.x / dt, dir.y / dt };
	pbody->body->SetLinearVelocity({ step.x, pbody->body->GetLinearVelocity().y });
	if (dir.y != 0 && canJump)
	{
		pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, -3.0f });
		counterJump = 0;
		canJump = false;
	}
}

bool Rat::SaveState(pugi::xml_node& data) const
{
	bool ret = true;

	pugi::xml_node alive = data.append_child("alive");
	alive.append_attribute("value").set_value(health);
		pugi::xml_node posN = data.append_child("position");
		posN.append_attribute("x").set_value(pos.x);
		posN.append_attribute("y").set_value(pos.y);
	return true;
}

bool Rat::LoadState(pugi::xml_node& data)
{
	if (health > 0)
	{
		health = data.child("eagle").attribute("health").as_int();
		pos.x = data.child("pos").attribute("x").as_int();
		pos.y = data.child("pos").attribute("y").as_int();
		//pbody->body->SetTransform({ PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y) }, 0.0f);
	}

	health = data.child("eagle").attribute("health").as_int();
	pos.x = data.child("pos").attribute("x").as_int();
	pos.y = data.child("pos").attribute("y").as_int();
	return true;
}
