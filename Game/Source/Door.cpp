#include "Door.h"

Door::Door(iPoint position_) : Entity(EntityType::DOOR, position_)
{
	this->w = 10;
	this->h = 10;
	pbody = app->physics->CreateRectangle(position.x, position.y, w, h, STATIC);
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

void Door::Update(float dt)
{
	if (this->IsActivated())
	{
		//TODO LOAD NEXT SCENE
	}
}
