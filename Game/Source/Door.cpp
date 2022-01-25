#include "Door.h"

Door::Door(iPoint position_) : Entity(EntityType::DOOR, position_)
{
	this->w = 16;
	this->h = 32;
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, w, h, STATIC);
	pbody->eListener = this;
	currentAnimation = &anim;
}

bool Door::IsActivated()
{
	return false;
}

void Door::Activate()
{
	this->activated = true;
}

void Door::Use()
{
	app->currentScene++;
}

void Door::Update(float dt)
{
}
