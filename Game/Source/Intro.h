#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "Animation.h"
#include "GuiButton.h"
#include "GuiCheck.h"
#include "GuiSlidder.h"
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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool SaveState(pugi::xml_node& data) const;
	bool LoadState(pugi::xml_node& data);

private:
	SDL_Texture* logoImg;
	SDL_Texture* background;
	SDL_Texture* credits;
	SDL_Texture* settings;

	Animation logoAnim;
	int frames = 0;
	SString folder;
	SString audioFile;
	SString guiFile;
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiButton* btn5;
	GuiButton* btn6;

	GuiCheck* check1;
	GuiCheck* check2;

	GuiSlidder* slid1;
	GuiSlidder* slid2;

	bool load;
	bool settingsShow = false;
	bool creditShow = false;
};

#endif // __SCENE_H__