#include "Gem.h"


Gem::Gem(iPoint position_, int ID_) : Entity(EntityType::GEM, position_)
{
	this->ID = ID_;
	name.Create("gem%i", ID);
	this->anim.PushBack({ 0, 1, 13, 11 });
	this->anim.PushBack({ 15, 1, 13, 11 });
	this->anim.PushBack({ 30, 1, 13, 11 });
	this->anim.PushBack({ 45, 1, 13, 11 });
	this->anim.PushBack({ 60, 1, 13, 11 });
	this->anim.loop = true;
	this->anim.speed = 0.1f;
	this->w = 13;
	this->h = 11;
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, w, h, STATIC);
	pbody->eListener = this;
	currentAnimation = &anim;
}

bool Gem::IsActivated()
{
	return this->active;
}

void Gem::Activate()
{
	active = true;
}

void Gem::Use()
{
	active = false;
	setPendingToDelete = true;
}

void Gem::Update(float dt)
{
	anim.Update();

	if (!this->IsActivated())
		this->setPendingToDelete = true;
}

