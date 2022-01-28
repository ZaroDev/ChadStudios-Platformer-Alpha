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

	inside = { 218,0,210,38 };
	outside = { 0,0,210,38 };
	knob = { 456,0,39,38 };

	minOutside = outside.x;
	maxOutside = outside.x + outside.w;


	//full.PushBack({ 0,0,210,38 });
	//empty.PushBack({ 218,0,210,38 });
	//slidebtn.PushBack({ 456,0,39,38 });
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
				//SliderControl(mouseX, mouseY);
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
	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		//render->DrawTexture(tex, bounds.x, bounds.y, &disabled.GetCurrentFrame());
	} break;

	case GuiControlState::NORMAL:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &inside);
		render->DrawTexture(tex, bounds.x, bounds.y, &outside);
		render->DrawTexture(tex, bounds.x, bounds.y, &knob);
		//draw
		knob.w *= value;
	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &inside);
		render->DrawTexture(tex, bounds.x, bounds.y, &outside);
		render->DrawTexture(tex, bounds.x, bounds.y, &knob);
		outside.w *= value;
	} break;
	case GuiControlState::PRESSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &inside);
		render->DrawTexture(tex, bounds.x, bounds.y, &outside);
		render->DrawTexture(tex, bounds.x, bounds.y, &knob);
		outside.w *= value;
	} break;
	default:
		break;
	}
	if (app->debug)
	{
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->DrawRectangle(bounds, 0, 0, 0, 0);
		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawRectangle(bounds, 255, 0, 0, 255);

		} break;

		//L14: TODO 4: Draw the button according the GuiControl State
		case GuiControlState::FOCUSED:
		{
			render->DrawRectangle(bounds, 255, 255, 255, 160);
		} break;
		case GuiControlState::PRESSED:
		{
			render->DrawRectangle(bounds, 255, 255, 255, 255);
		} break;

		/******/

		case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
			break;

		default:
			break;
		}
	}

	return false;
}

void GuiSlidder::SliderControl(int mouseX, int mouseY)
{
	knob.x = mouseX - (knob.w / 2);

	value = ((maxValue - minValue) * (mouseX - (float)(outside.x + knob.w / 2))) / (float)(outside.w - knob.w) + minValue;

	// Limits
	if (knob.x < outside.x)
	{
		knob.x = outside.x;
		value = minValue;
	}
	if ((knob.x + knob.w) > (outside.x + outside.w))
	{
		knob.x = (outside.x + outside.w) - knob.w;
		value = maxValue;
	}
}