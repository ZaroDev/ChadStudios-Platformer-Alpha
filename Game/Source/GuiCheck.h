#ifndef __GUICHECK_H__
#define __GUICHECK_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"

class SDL_Texture;

class GuiCheck : public GuiControl
{
public:

	GuiCheck(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheck();

	bool Update(float dt);
	bool Draw(Render* render, SDL_Texture* tex);

	int mouseX, mouseY;
	unsigned int click;

	bool checked;
	
	Animation pressed;
	Animation focused;
	Animation normal;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICHECK_H__