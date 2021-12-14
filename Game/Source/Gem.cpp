#include "Gem.h"

Gem::Gem() : Entity()
{
	anim.PushBack({ 0, 1, 13, 11 });
	anim.PushBack({ 15, 1, 13, 11 });
	anim.PushBack({ 30, 1, 13, 11 });
	anim.PushBack({ 45, 1, 13, 11 });
	anim.PushBack({ 60, 1, 13, 11 });
	anim.loop = true;
	anim.speed = 0.1f;
}

Gem::Gem(iPoint position_) : Entity(EntityType::GEM, position_)
{
	anim.PushBack({ 0, 1, 13, 11 });
	anim.PushBack({ 15, 1, 13, 11 });
	anim.PushBack({ 30, 1, 13, 11 });
	anim.PushBack({ 45, 1, 13, 11 });
	anim.PushBack({ 60, 1, 13, 11 });
	anim.loop = true;
	anim.speed = 0.1f;
}

Gem::~Gem()
{
}

bool Gem::IsActivated()
{
	return activated;
}

void Gem::Activate()
{
	activated = true;
}

void Gem::Update(float dt)
{
	anim.Update();
}
