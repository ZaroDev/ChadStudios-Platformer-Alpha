#include "Door.h"

Door::Door(iPoint position_, int ID_) : Entity(EntityType::DOOR, position_)
{
	this->ID = ID_;
	name.Create("door%i", ID);
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
}

void Door::Use()
{
	setPendingToDelete = true;
}

void Door::Update(float dt)
{
}


