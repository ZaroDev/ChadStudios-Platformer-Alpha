#pragma once
#include "Entity.h"

class Cherry : public Entity
{
public:
	Cherry();
	Cherry(iPoint position_);
	~Cherry();

	bool IsActivated();
	void Activate();

	void Update(float dt);

private:
	bool activated = false;
};