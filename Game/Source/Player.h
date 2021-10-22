#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;



class Player : public Module
{
public:
	Player(bool active);
	~Player();

	bool Awake(pugi::xml_node&);

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	iPoint pos;

	PhysBody* pbody;

	int speed = 1;

	SDL_Texture* tex;

	Animation* currentAnimation = nullptr;

	Animation idleAnimL;
	Animation idleAnimR;

	Animation runAnimL;
	Animation runAnimR;

	Animation jumpAnimL;
	Animation jumpAnimR;
	bool grounded;
};