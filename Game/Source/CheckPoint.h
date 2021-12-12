#include "Point.h"
#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "List.h"


struct SDL_Texture;

struct Flag
{
	iPoint pos;
	PhysBody* body;
	bool isActive = true;
	Animation flagAnim;
};


class CheckPoint : public Module
{
public:
	CheckPoint(bool startEnabled);
	~CheckPoint();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateCheckpoint(int x, int y);
private:
	SDL_Texture* tex;
	int SFX;
	List<Flag*> checkpoints;
	SString folder;
	SString sfx;
};