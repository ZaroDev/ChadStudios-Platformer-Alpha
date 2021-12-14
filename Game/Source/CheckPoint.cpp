#include "CheckPoint.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"
#include "Audio.h"

CheckPoint::CheckPoint()
{
}

CheckPoint::CheckPoint(iPoint position_) : Entity(EntityType::CHECKPOINT, position_)
{
	anim.PushBack({ 64, 0, 32, 53 });
	anim.PushBack({ 32, 0, 32, 53 });
	anim.PushBack({ 0, 0, 32, 53 });
	anim.speed = 0.1f;
	anim.loop = false;
}

CheckPoint::~CheckPoint()
{
}

bool CheckPoint::IsActivated()
{
	return activated;
}

void CheckPoint::Activate()
{
	activated = true;
}






void CheckPoint::Update(float dt)
{
	if (activated && !anim.HasFinished())
		anim.Update();
}





