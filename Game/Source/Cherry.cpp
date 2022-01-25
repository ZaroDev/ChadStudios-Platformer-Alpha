#include "Cherry.h"


Cherry::Cherry(iPoint position_, int ID_) : Entity(EntityType::CHERRY, position_)
{
	this->ID = ID_;
	name.Create("cherry%i", ID);
	this->anim.PushBack({ 0, 14, 16, 16 });
	this->anim.PushBack({ 22, 14, 16, 16 });
	this->anim.PushBack({ 44, 14, 16, 16 });
	this->anim.PushBack({ 67, 14, 16, 16 });
	this->anim.loop = true;
	this->anim.speed = 0.1f;
	this->w = 16;
	this->h = 16;
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, w, h, STATIC);
	pbody->eListener = this;
	currentAnimation = &anim;
}

bool Cherry::IsActivated()
{
	return this->active;
}

void Cherry::Activate()
{
	this->active = true;
}

void Cherry::Update(float dt)
{
	anim.Update();
	if (!this->IsActivated())
		setPendingToDelete = true;
	
}

void Cherry::Use()
{
	active = false;
	setPendingToDelete = true;
}


