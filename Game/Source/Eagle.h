#pragma once
#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle();
	~Eagle();

	void Update() override;

};