#include "CheckPoint.h"

#include "App.h"
#include "Render.h"
#include "Audio.h"


CheckPoint::CheckPoint(iPoint position_, int ID_) : Entity(EntityType::CHECKPOINT, position_)
{
	this->ID = ID_;
	name.Create("checkpoint%i", ID);
	anim.PushBack({ 64, 0, 32, 53 });
	anim.PushBack({ 32, 0, 32, 53 });
	anim.PushBack({ 0, 0, 32, 53 });
	anim.speed = 0.1f;
	anim.loop = false;
	this->w = 32;
	this->h = 53;
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, w, h, STATIC);
	pbody->eListener = this;
	this->active = false;
	currentAnimation = &anim;
}

bool CheckPoint::IsActivated()
{
	return active;
}

void CheckPoint::Activate()
{
	active = true;
}

void CheckPoint::Update(float dt)
{
	if (this->IsActivated() && !anim.HasFinished())
		anim.Update();

}

void CheckPoint::Use()
{
	this->Activate();
}







