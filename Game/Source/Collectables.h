#include "Module.h"
#include "List.h"
#include "Animation.h"
#include "App.h"
#include "Point.h"
#include "Physics.h"
#include "SString.h"

struct SDL_Texture;

enum Type
{
	DEFAULT = -1,
	GEM,
	CHERRY,
};

struct Collectable
{
	bool pendingToDelete = false;
	iPoint pos;
	Type type;
	PhysBody* body;
	//The amount of life/score that gives to the player
	Animation anim;
};



class Collectables : public Module
{
public:
	Collectables(bool startEnabled);
	~Collectables();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateObj(Type type, float x, float y);

private:
	SString folder;
	SDL_Texture* tex;
	List<Collectable*> colectables;
};
