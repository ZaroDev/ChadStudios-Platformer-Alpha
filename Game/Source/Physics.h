#pragma once
#include "Module.h"


class Physics : public Module
{
public:
	Physics();
	~Physics();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

private:

	bool debug;
};