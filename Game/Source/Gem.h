#pragma once
#include "Entity.h"

class Gem : public Entity
{
public:
	Gem(iPoint position_, int ID);


	bool IsActivated();
	void Activate();

	void Use();

	void Update(float dt);
};