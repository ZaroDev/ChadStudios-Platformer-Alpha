#pragma once
#include "Entity.h"

class Gem : public Entity
{
public:
	Gem();
	Gem(iPoint position_);
	~Gem();

	bool IsActivated();
	void Activate();

	void Update(float dt);
private:
	bool activated = false;
};