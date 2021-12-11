#include "Eagle.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "SDL/include/SDL.h"

Eagle::Eagle() : Enemy()
{
	anim.PushBack({ 0,28,38,42 });
	anim.PushBack({ 40,31,40,40 });
	anim.PushBack({ 80,40,32,30 });
	anim.PushBack({ 117,40,40,30 });
	anim.speed = 0.1f;
	anim.loop = true;
	h = 16;
	w = 16;
	health = 1;
	range = 5;
}

Eagle::~Eagle()
{
}


void Eagle::Update()
{
	anim.Update();

	hasTarget = CheckIfHasTarget();

	//The enemy has only to move if it's in range of the player
	if (hasTarget && health > 0)
	{
		ComputePath();
		MoveToPlayer();
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

void Eagle::ComputePath()
{
	iPoint origin = { (int)METERS_TO_PIXELS((int)pbody->body->GetPosition().x), (int)METERS_TO_PIXELS((int)pbody->body->GetPosition().y) };
	iPoint dest = { (int)METERS_TO_PIXELS((int)app->player->pbody->body->GetPosition().x + 12), (int)METERS_TO_PIXELS((int)app->player->pbody->body->GetPosition().y + 12) };

	origin = app->map->WorldToMap(origin.x, origin.y);
	dest = app->map->WorldToMap(dest.x, dest.y);

	app->pathfinding->CreatePath(origin, dest);
	currentPath = app->pathfinding->GetLastPath();
}

void Eagle::MoveToPlayer()
{
	if (currentPath->At(1)->x != NULL)
	{
		iPoint nextStep = { currentPath->At(1)->x, currentPath->At(1)->y };
		nextStep = app->map->MapToWorld(nextStep.x, nextStep.y);
		nextStep.x = PIXEL_TO_METERS(nextStep.x);
		nextStep.y = PIXEL_TO_METERS(nextStep.y);

		int posX = pbody->body->GetPosition().x;
		int posY = pbody->body->GetPosition().y;
		printf("\nposx: %i, %i", posX, nextStep.x);
		printf("\nposy: %i, %i", posY, nextStep.y);
		if (posX < nextStep.x)
		{
			if (posY > nextStep.y)
			{
				pbody->body->SetLinearVelocity({ speed.x, speed.y });
				printf("up\n");
			}
			else if (posY < nextStep.y)
			{
				pbody->body->SetLinearVelocity({ speed.x, -speed.y });
				printf("down\n");
			}
		}
		else if (posX >= nextStep.x) 
		{
			if (posY > nextStep.y)
			{
				pbody->body->SetLinearVelocity({ -speed.x, speed.y });
				printf("up\n");
			}
			else if (posY < nextStep.y)
			{
				pbody->body->SetLinearVelocity({ -speed.x, -speed.y });
				printf("down\n");
			}
		}


		if (posY == nextStep.y)
		{
			pbody->body->SetLinearVelocity({ pbody->body->GetLinearVelocity().x, 0 });
			printf("equal\n");
		}
		if (posX == nextStep.x) pbody->body->SetLinearVelocity({ 0, pbody->body->GetLinearVelocity().y });
	}
	
}
