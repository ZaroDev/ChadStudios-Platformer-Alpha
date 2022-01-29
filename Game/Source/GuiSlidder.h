#ifndef __GUISLIDDER_H__
#define __GUISLIDDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Audio.h"

class GuiSlidder : public GuiControl
{
public:

	GuiSlidder(uint32 id, SDL_Rect bounds, const char* text, int min = 0, int max = 100);
	virtual ~GuiSlidder();

	bool Update(float dt);
	bool Draw(Render* render, SDL_Texture* tex);

	void SliderControl(int mouseX, int mouseY);

	int GetValue() { return value; };
	void SetValue(int newValue);
	

	int mouseX, mouseY;
	unsigned int click;
	
	//IDK
	int margin = 10;
	int marginSliders = 4;

	float value;
	int lastValue;

	int minValue;
	int maxValue;

	int maxOutside;
	int minOutside;

	/*Animation empty;
	Animation full;
	Animation slidebtn;*/

	iPoint knobPos;
	SDL_Rect inside;
	SDL_Rect outside;
	SDL_Rect knob;

	int onClickFX;
	int onHoverFX;
	bool isPlaying;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUISLIDER_H__