#include "Point.h"
#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "List.h"


struct SDL_Texture;

struct Flag
{
	SDL_Texture* tex;
	PhysBody* body;
	Animation flagAnim;
	Module* listener;
};


class CheckPoint : public Module
{
public:
	CheckPoint(bool startEnabled);
	~CheckPoint();

	bool Awake(pugi::xml_node&);

	void CreateCheckpoint(int x, int y);
	List<Flag>* checkpoints;
};