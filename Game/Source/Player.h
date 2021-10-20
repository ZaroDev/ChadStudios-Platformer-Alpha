#include "Module.h"
#include "Point.h"
#include "Animation.h"


struct SDL_Texture;

class Player : public Module
{
public:
	Player(bool active);
	~Player();

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	iPoint pos;

	int speed;

	SDL_Texture* tex;

	Animation* currentAnimation = nullptr;

	Animation idleAnimL;
	Animation idleAnimR;

	Animation runAnimL;
	Animation runAnimR;

	Animation jumpAnimL;
	Animation jumpAnimR;
};