#include "Rat.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "SDL/include/SDL.h"

Rat::Rat(iPoint position_, Entity* target) : Enemy(EntityType::ENEMY_RAT, position_, target)
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
	pbody = app->physics->CreateRectangle(position.x, position.y, w, h, DYNAMIC);
	pbody->eListener = this;
	currentAnimation = &anim;
	range = 200;
	pathUpdateTime = 1.5f;
	pathUpdateTimer = pathUpdateTime;
}



void Rat::Update(float dt)
{
	if (this->health <= 0)
	{
		this->setPendingToDelete = true;
		return;
	}

	anim.Update();
	
	hasTarget = CheckIfHasTarget();
	position.x = METERS_TO_PIXELS(pbody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pbody->body->GetPosition().y);
	//The enemy has only to move if it's in range of the player
	if (hasTarget && health > 0 && target->GetHealth() > 0)
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
	if (target == nullptr) return;
	iPoint playerPos = target->GetPos();
	float dist = Distance(position.x, position.y, playerPos.x, playerPos.y);

	pathUpdateTimer += dt;
	if (dist > range) {
		return;
	}
	else
	{
		if (target->GetState() != EntityState::HURT)
		{
			if (pathUpdateTimer >= pathUpdateTime) {
				pathUpdateTimer = 0.0f;
				pathIndex = 0;

				iPoint origin = app->map->WorldToMap(position.x, position.y);
				iPoint destination = app->map->WorldToMap(target->GetPos().x, target->GetPos().y);
				int res = app->pathfinding->CreatePath(origin, destination);

				if (res > 0) {

					currentPath = app->pathfinding->GetLastPath();
					if (currentPath != nullptr)
					{
						if (currentPath->Count() > 1)
						{
							pathIndex = 1;
							activeNode = app->map->MapToWorld(currentPath->At(pathIndex)->x, currentPath->At(pathIndex)->y);
						}
						else if (currentPath->Count() > 0)
						{
							activeNode = app->map->MapToWorld(currentPath->At(0)->x, currentPath->At(0)->y);
						}
					}
				}
				if (currentPath != nullptr)
				{
					if (currentPath->Count() > 0) {
						if (position == activeNode) {
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
			}
		}
	}
	if (!canJump)
	{
		counterJump++;
		if (counterJump >= 120)
			canJump = true;
	}
}

void Rat::MoveToPlayer(iPoint destination,float dt)
{
	iPoint diff = destination - position;

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
