#ifndef __UI_H__
#define __UI_H__

#include "Module.h"
#include "Animation.h"
#include "App.h"
#include "GuiButton.h"

struct SDL_Texture;
class UI : public Module
{
public:
	UI(bool startEnabled);
	~UI();
	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void AddScore(int score);

	void ResetScore();

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&);
	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);
private:
	SDL_Texture* heart;
	SDL_Texture* gem;
	SDL_Texture* anim;
	SDL_Texture* originTex;
	SDL_Texture* pausetex;
	SDL_Texture* settings;
	bool onPause = true;
	int counter = 0;
	float seconds = 0;
	int minutes = 0;
	int score = 0;
	int font;
	int scoreMult = 1;
	Animation heartAnim;
	Animation gemAnim;
	Animation abAnim;
	int highScore = 0;
	SString folder;
	bool settingsShow = false;
	bool pauseShow = false;
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiButton* btn5;
};

#endif // __UI_H__