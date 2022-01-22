#include "Gem.h"


Gem::Gem(iPoint position_) : Entity(EntityType::GEM, position_)
{
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
	return this->activated;
}

void Gem::Activate()
{
	this->activated = true;
}

void Gem::Use()
{
	this->Activate();
}

void Gem::Update(float dt)
{
	anim.Update();

	if (this->IsActivated())
		this->setPendingToDelete = true;
}
