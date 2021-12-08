#include "App.h"
#include "Physics.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class Enemy
{
public:
	Enemy();
	~Enemy();

	iPoint GetPos();
	void Update();

private:
	iPoint pos;
	Animation anim;
	SDL_Texture* tex;
	PhysBody* pbody;
};