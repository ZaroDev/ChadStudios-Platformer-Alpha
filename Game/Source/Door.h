#pragma once
#include "Entity.h"

class Door : public Entity
{
public:
	Door(iPoint position_);

	bool IsActivated();
	void Activate();

	void Use();

	void Update(float dt);
private:
	bool activated = false;
};