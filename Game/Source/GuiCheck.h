#ifndef __GUICHECK_H__
#define __GUICHECK_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Audio.h"

class SDL_Texture;

class GuiCheck : public GuiControl
{
public:

	GuiCheck(uint32 id, SDL_Rect bounds, const char* text, bool initState = false);
	virtual ~GuiCheck();

	bool Update(float dt);
	bool Draw(Render* render, SDL_Texture* tex);

	bool GetValue() { return checked; };
	void SetValue(bool state) { checked = state; };

	int mouseX, mouseY;
	unsigned int click;

	bool checked;
	
	Animation pressed;
	Animation focused;
	Animation normal;
	Animation xcheck;


	int onClickFX;
	int onHoverFX;
	bool isPlaying;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICHECK_H__