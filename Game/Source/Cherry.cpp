#include "Cherry.h"


Cherry::Cherry(iPoint position_) : Entity(EntityType::CHERRY, position_)
{
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
	return this->activated;
}

void Cherry::Activate()
{
	this->activated = true;
}

void Cherry::Update(float dt)
{
	anim.Update();
	if (this->IsActivated())
		this->setPendingToDelete = true;
}

void Cherry::Use()
{
	this->Activate();
}
