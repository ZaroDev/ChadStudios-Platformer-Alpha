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
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateEnemy(EnemyType type, float x, float y);
private:
	List<Enemy*> enemies;
	SString folder;
	SDL_Texture* tex;
};