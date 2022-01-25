#include "Eagle.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "SDL/include/SDL.h"

Eagle::Eagle(iPoint position_, Entity* target) : Enemy(EntityType::ENEMY_EAGLE, position_, target)
{
	anim.PushBack({ 0,28,38,42 });
	anim.PushBack({ 40,31,40,40 });
	anim.PushBack({ 80,40,32,30 });
	anim.PushBack({ 117,40,40,30 });
	anim.speed = 0.1f;
	anim.loop = true;
	this->h = 25;
	this->w = 25;
	pbody = app->physics->CreateRectangle(position.x, position.y, w, h, KINEMATIC);
	pbody->eListener = this;
	pbody->body->SetFixedRotation(true);
	currentAnimation = &anim;
	health = 1;
	range = 300;
	pathUpdateTime = 1.5f;
	pathUpdateTimer = pathUpdateTime;
}

void Eagle::Update(float dt)
{
	if (this->health <= 0)
	{
		this->setPendingToDelete = true;
		return;
	}

	anim.Update();

	hasTarget = CheckIfHasTarget();

	//The enemy has only to move if it's in range of the player
	position.x = METERS_TO_PIXELS(pbody->body->GetPosition().x);
	position.y = METERS_TO_PIXELS(pbody->body->GetPosition().y);
	if (hasTarget && health > 0 && target->GetHealth() > 0)
	{
		ComputePath(dt);
	}
	else
	{
		pbody->body->SetLinearVelocity({ 0, 0 });
		currentPath = nullptr;
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

void Eagle::ComputePath(float dt)
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
	
}

void Eagle::MoveToPlayer(iPoint destination, float dt)
{
	iPoint diff = destination - position;
	
	fPoint dir = { (float)diff.x, (float)diff.y };
	dir.Normalize();
	dir *= speed * 4;

	fPoint step = { dir.x / dt, dir.y / dt };
	pbody->body->SetLinearVelocity({ step.x, step.y });
}
