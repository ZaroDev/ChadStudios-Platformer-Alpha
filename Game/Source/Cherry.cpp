#include "Cherry.h"

Cherry::Cherry() : Entity()
{
	anim.PushBack({ 0, 14, 16, 16 });
	anim.PushBack({ 22, 14, 16, 16 });
	anim.PushBack({ 44, 14, 16, 16 });
	anim.PushBack({ 67, 14, 16, 16 });
	anim.loop = true;
	anim.speed = 0.1f;
}

Cherry::Cherry(iPoint position_) : Entity(EntityType::CHERRY, position_)
{
	anim.PushBack({ 0, 14, 16, 16 });
	anim.PushBack({ 22, 14, 16, 16 });
	anim.PushBack({ 44, 14, 16, 16 });
	anim.PushBack({ 67, 14, 16, 16 });
	anim.loop = true;
	anim.speed = 0.1f;
}

Cherry::~Cherry()
{
}

bool Cherry::IsActivated()
{
	return activated;
}

void Cherry::Activate()
{
	activated = true;
}

void Cherry::Update(float dt)
{
	anim.Update();
}
