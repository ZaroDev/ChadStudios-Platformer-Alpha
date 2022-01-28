#include "GuiCheck.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

#include "GuiSlidder.h"
#include "Window.h"

GuiSlidder::GuiSlidder(uint32 id, SDL_Rect bounds, const char* text, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	//TODO Poner las animaciones

	canClick = true;
	drawBasic = false;

	if (min > max)
	{
		minValue = max;
		maxValue = min;
	}
	else
	{
		minValue = min;
		maxValue = max;
	}


	full.PushBack({ 0,0,210,38 });
	empty.PushBack({ 218,0,210,38 });
	slidebtn.PushBack({ 456,0,39,38 });
}

GuiSlidder::~GuiSlidder()
{

}

bool GuiSlidder::Update(float dt)
{
	bool ret = true;
	if (state != GuiControlState::DISABLED)
	{
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		int offsetX = -app->render->camera.x / app->win->GetScale();
		int offsetY = -app->render->camera.y / app->win->GetScale();
		if ((mouseX + offsetX > bounds.x) && (mouseX + offsetX < (bounds.x + bounds.w)) &&
			(mouseY + offsetY > bounds.y) && (mouseY + offsetY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{				
				ret = NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return ret;
}

bool GuiSlidder::Draw(Render* render, SDL_Texture* tex)
{
	return false;
}