#include "CheckPoint.h"

#include "App.h"
#include "Render.h"
#include "Audio.h"


CheckPoint::CheckPoint(iPoint position_) : Entity(EntityType::CHECKPOINT, position_)
{
	anim.PushBack({ 64, 0, 32, 53 });
	anim.PushBack({ 32, 0, 32, 53 });
	anim.PushBack({ 0, 0, 32, 53 });
	anim.speed = 0.1f;
	anim.loop = false;
	this->w = 32;
	this->h = 53;
	pbody = app->physics->CreateRectangle(position.x, position.y, w, h, STATIC);
	pbody->eListener = this;
	currentAnimation = &anim;
}

bool CheckPoint::IsActivated()
{
	return activated;
}

void CheckPoint::Activate()
{
	activated = true;
}

void CheckPoint::Update(float dt)
{
	if (activated && !anim.HasFinished())
		anim.Update();
}

void CheckPoint::Use()
{
	this->Activate();
}





