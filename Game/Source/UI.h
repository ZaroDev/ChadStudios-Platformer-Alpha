#ifndef __UI_H__
#define __UI_H__

#include "Module.h"
#include "Animation.h"
#include "App.h"

struct SDL_Texture;
class UI : public Module
{
public:
	UI(bool startEnabled);
	~UI();
	bool Awake(pugi::xml_node&);

	bool Start();

	bool PostUpdate();

	bool CleanUp();


private:
	SDL_Texture* heart;
	SDL_Texture* gem;
	SDL_Texture* anim;
	
	int font;

	Animation heartAnim;
	Animation gemAnim;
	Animation abAnim;

	SString folder;
};

#endif // __UI_H__