#include "Eagle.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"

Eagle::Eagle() : Enemy()
{
	anim.PushBack({ 0,28,38,42 });
	anim.PushBack({ 40,31,40,40 });
	anim.PushBack({ 80,40,32,30 });
	anim.PushBack({ 117,40,40,30 });
	anim.speed = 0.1f;
	anim.loop = true;
	posCheckTime = 8;
	posCheckTimeAgro = 15;
	maxDistanceAgroBase = 8;
	maxDistanceAgroActive = 11;

	agroSpeed.x = 3.f;
	agroSpeed.y = 3.f;
	calmSpeed.x = 3.f;
	calmSpeed.y = 3.f;
	startPosMargin = 48 + 24;
	h = 30;
	w = 30;
	health = 1;
	agroSpeed = b2Vec2(1, 1);
}

Eagle::~Eagle()
{
}


void Eagle::Update()
{
	anim.Update();

	iPoint myPosWorld(
		METERS_TO_PIXELS(pbody->body->GetPosition().x),
		METERS_TO_PIXELS(pbody->body->GetPosition().y)
	);
	iPoint playerPosWorld(
		METERS_TO_PIXELS(app->player->pbody->body->GetPosition().x),
		METERS_TO_PIXELS(app->player->pbody->body->GetPosition().y)
	);
	if (agroTowardsPlayer)
	{
		if (CheckDistanceToPhysBody(app->player->pbody) <= maxDistanceAgroBase)
		{
			agroTowardsPlayer = true;
			maxDistanceAgro = maxDistanceAgroActive;
		}
		else {
			agroTowardsPlayer = false;
			maxDistanceAgro = maxDistanceAgroBase;
		}
	}
	else
	{
		if (CheckDistanceToPhysBody(app->player->pbody) <= maxDistanceAgroBase)
		{
			agroTowardsPlayer = true;
			maxDistanceAgro = maxDistanceAgroActive;

		}
		else {
			agroTowardsPlayer = false;
			maxDistanceAgro = maxDistanceAgroBase;

		}
	}
	//LOG("agro distance %i", maxDistanceAgro);

	//LOG("agro %i", agroTowardsPlayer);
	//LOG("distance: %i", CheckDistanceToPhysBody(app->player->GetColHitbox()));
	iPoint myPosMap(app->map->WorldToMap(myPosWorld.x, myPosWorld.y));
	iPoint playerPosMap(app->map->WorldToMap(playerPosWorld.x, playerPosWorld.y));

	if (agroTowardsPlayer)
	{
		if (checkTimerAgro == posCheckTimeAgro)
		{
			//LOG("agro timer working");
			checkTimer = 0;
			app->pathfinding->CreatePath(myPosMap, playerPosMap);

			//LOG("mypos: %i, %i", myPosMap.x, myPosMap.y);
			//LOG("playerpos: %i, %i", playerPosMap.x, playerPosMap.y);





			checkTimerAgro = 0;
		}
		checkTimerAgro++;

		//LOG("target pos: %i, %i", nextMovePos.x, nextMovePos.y);

	}
	

	const DynArray<iPoint>* tempPath = app->pathfinding->GetLastPath();
	if (tempPath->Count() > 2)
	{
		iPoint temp(tempPath->At(1)->x, tempPath->At(1)->y);
		nextMovePos = temp;


	}


	nextMovePos = app->map->MapToWorld(nextMovePos.x, nextMovePos.y);

	b2Vec2 direction(
		nextMovePos.x + 24 - METERS_TO_PIXELS(pbody->body->GetPosition().x),
		nextMovePos.y + 24 - METERS_TO_PIXELS(pbody->body->GetPosition().y)
	);

	direction.Normalize();
	if (agroTowardsPlayer)
	{

		currentSpeed.x = agroSpeed.x * direction.x;
		currentSpeed.y = agroSpeed.y * direction.y;
	}
	else {
		if (!inSpawnPos)
		{
			currentSpeed.x = calmSpeed.x * direction.x;
			currentSpeed.y = calmSpeed.y * direction.y;
		}
		else {
			currentSpeed.x = 0;
			currentSpeed.y = 0;
		}
	}
	//LOG("next pos world %i", nextMovePos.y + 24);
	//LOG("pos, %i", METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("res: %i", nextMovePos.y + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	//LOG("res: %i", METERS_TO_PIXELS(direction.y));
	//LOG("res: %i", METERS_TO_PIXELS(calmSpeed.y));
	//LOG("res: %i", METERS_TO_PIXELS(currentSpeed.y));
	//LOG("spawn: %i, %i", nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().x), nextMovePos.x + 24 - METERS_TO_PIXELS(Hitbox->body->GetPosition().y));
	pbody->body->SetLinearVelocity(currentSpeed);
}
