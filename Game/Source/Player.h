#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"
#include "FadeToBlack.h"

struct SDL_Texture;



class Player : public Module
{
public:
	Player(bool startEnabled);
	~Player();

	bool Awake(pugi::xml_node&);

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp() override;

	iPoint pos;

	PhysBody* pbody;

	SDL_Texture* tex;

	Animation* currentAnimation = nullptr;

	Animation idleAnimL;
	Animation idleAnimR;

	Animation runAnimL;
	Animation runAnimR;

	Animation jumpAnimL;
	Animation downAnimL;
	Animation jumpAnimR;
	Animation downAnimR;

	int jumpSFX;
	int currentScene;
	bool grounded;
	int numJumps;
	float minVel;
	float maxVel;
	float jumpVel;
	SString folder;
	iPoint scene1;
	iPoint scene2;
	SString jumpSFXFile;
	bool die;

	bool debug;
};