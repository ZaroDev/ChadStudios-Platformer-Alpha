#pragma once
#include "Module.h"
#include "Enemy.h"
#include "List.h"

enum EnemyType
{
	RAT = -1,
	EAGLE,

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void CreateEnemy(EnemyType type, float x, float y, int health);
private:
	List<Enemy*> enemies;
	int hitSFX;
	int playerHit;
	SString sfx;
	SString folder;
	SDL_Texture* tex;
	SDL_Texture* path;
};