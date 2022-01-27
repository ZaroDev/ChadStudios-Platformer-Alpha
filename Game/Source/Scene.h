#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "GuiButton.h"


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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Called before quitting
	bool CleanUp();

	

private:
	SDL_Texture* background;
	SDL_Texture* jungle;
	SDL_Texture* pathTex;
	SDL_Texture* originTex;
	SDL_Texture* pausetex;
	SDL_Texture* settings;
	
	SString folder;
	SString audioFile;
	int startX;
	int startY;

	iPoint origin;
	bool originSelected = false;

	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;

	bool settingsShow = false;
	bool pauseShow = false;
};

#endif // __SCENE_H__