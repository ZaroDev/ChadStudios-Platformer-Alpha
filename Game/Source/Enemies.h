#pragma once
#include "Module.h"
#include "Enemy.h"
#include "List.h"

enum EnemyType
{
	EAGLE,
	RAT,

	MAX
};

class Enemies : public Module
{
public:
	Enemies(bool startEnabled);
	~Enemies();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void CreateEnemy(EnemyType type, float x, float y);
private:
	List<Enemy*> enemies;
};