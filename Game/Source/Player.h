#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"
#include "FadeToBlack.h"
#include "List.h"
struct SDL_Texture;

struct Sensor {
	enum sensorValue
	{
		NONE = -1,
		LEFT,
		RIGHT
	};
	PhysBody* sensor;
	sensorValue value;
	bool isActive;
	int sensorTimer = 0;
};

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
	void CreateSensor(PhysBody* sensor, Sensor::sensorValue sensorType, bool isActive);
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

	Animation hurtAnim;
	List<Sensor*>sensors;

	int jumpSFX;
	int currentScene;
	bool grounded;
	int numJumps;
	int lives;
	int score;
	float minVel;
	float maxVel;
	float jumpVel;
	SString folder;
	iPoint scene1;
	iPoint scene2;
	SString jumpSFXFile;
	int counter;
	bool hurt;
	bool die;
	bool win;
	b2Body* c;
	bool debug;
};