#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
class Tile
{
public:
	Tile() {};
	~Tile() {};
	enum Type
	{
		NONE = -1,
		GROUND,

	};
	Type type;
	PhysBody* body;
};
class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	SDL_Texture* img;
	SDL_Texture* background;
	SDL_Texture* jungle;

	List<Tile*> ground;
};

#endif // __SCENE_H__