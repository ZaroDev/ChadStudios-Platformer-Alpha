#ifndef _DEATH_H_
#define _DEATH_H_

#include "Module.h"
#include "Animation.h"
struct SDL_Texture;

class Death : public Module
{
public:

	Death(bool startEnabled);

	// Destructor
	virtual ~Death();

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
	//SDL_Texture* logoImg;
	SDL_Texture* backgroundDeath;
	SDL_Texture* deathImg;
	SDL_Texture* backgroundWin;
	SDL_Texture* winImg;

	Animation deathAnim;
	Animation winAnim;
	int frames = 0;
	SString folder;
	SString audioFile;
};

#endif // _SCENE_H_