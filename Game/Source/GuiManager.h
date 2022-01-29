#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(bool startEnabled);

	// Destructor
	virtual ~GuiManager();

	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool UpdateAll(float dt, bool logic);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type,GuiButtonType btype,  int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });
	void DestroyGuiControl(GuiControl* entity);
	void AddGuiControl(GuiControl* entity);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;

	SDL_Texture* playTex;
	SDL_Texture* continueTex;
	SDL_Texture* settingsTex;
	SDL_Texture* creditsTex;
	SDL_Texture* exitTex;
	SDL_Texture* resumeTex;
	SDL_Texture* bttTex;
	SDL_Texture* closeTex;

	SDL_Texture* checkbox;

	SDL_Texture* slider;

	SString folder;
};

#endif // __GUIMANAGER_H__
