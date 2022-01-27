#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	

private:
	SDL_Texture* background;
	SDL_Texture* jungle;
	SDL_Texture* pathTex;

	
	SString folder;
	SString audioFile;
	int startX;
	int startY;

	iPoint origin;
	bool originSelected = false;




};

#endif // __SCENE_H__