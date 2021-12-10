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
	void SetPos(float x, float y);
	virtual void Update();

	SDL_Texture* tex;
	Animation anim;
private:
	iPoint pos;
	PhysBody* pbody;
};