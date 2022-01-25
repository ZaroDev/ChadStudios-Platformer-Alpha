#pragma once
#include "Entity.h"
#include "Animation.h"

class CheckPoint : public Entity
{
public:
	CheckPoint(iPoint position_, int ID);

	bool IsActivated();
	void Activate();

	void Update(float dt);
	void Use();
};