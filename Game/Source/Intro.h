#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "Animation.h"
struct SDL_Texture;

class Intro : public Module
{
public:

	Intro(bool startEnabled);

	// Destructor
	virtual ~Intro();

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
	SDL_Texture* logoImg;
	SDL_Texture* background;
	SDL_Texture* enterImg;
	Animation logoAnim;
	int frames = 0;
	SString folder;
	SString audioFile;
};

#endif // __SCENE_H__