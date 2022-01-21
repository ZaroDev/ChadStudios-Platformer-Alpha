#pragma once
#include "Entity.h"

class Cherry : public Entity
{
public:
	Cherry(iPoint position_);


	bool IsActivated();
	void Activate();

	void Update(float dt);
	void Use();
private:
	bool activated = false;
};