#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Audio.h"

class SDL_Texture;

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, GuiButtonType btype, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render, SDL_Texture* tex);

	int mouseX, mouseY;
	unsigned int click;

	
	Animation pressed;
	Animation focused;
	Animation normal;
	Animation disabled;
	Animation noDraw;
	bool UI = false;

	int onClickFX;
	int onHoverFX;
	bool isPlaying;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUIBUTTON_H__