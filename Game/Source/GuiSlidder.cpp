#include "GuiCheck.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

#include "GuiSlidder.h"
#include "Window.h"
#include "Log.h"
GuiSlidder::GuiSlidder(uint32 id, SDL_Rect bounds, const char* text, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	//TODO Poner las animaciones
	knobPos.x = bounds.x;
	knobPos.y = bounds.y;
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
	value = 0.0f;
	outside = { 218,0,210,38 };
	inside = { 0,0,210,38 };
	knob = { 456,0,39,38 };
	knobPos.x = bounds.x;
	minOutside = outside.x;
	maxOutside = outside.x + outside.w;

	onClickFX = app->audio->LoadFx("Assets/audio/fx/onClickFX.wav");
	onHoverFX = app->audio->LoadFx("Assets/audio/fx/onHoverFX.wav");
}

GuiSlidder::~GuiSlidder()
{

}

bool GuiSlidder::Update(float dt)
{

	bool ret = true;
	if (state != GuiControlState::DISABLED)
	{
		knobPos.y = bounds.y;
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		int offsetX = -app->render->camera.x / app->win->GetScale();
		int offsetY = -app->render->camera.y / app->win->GetScale();
		if ((mouseX + offsetX > bounds.x) && (mouseX + offsetX < (bounds.x + bounds.w)) &&
			(mouseY + offsetY > bounds.y) && (mouseY + offsetY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			if (isPlaying == false)
			{
				app->audio->PlayFx(onHoverFX);
				isPlaying = true;
			}
			if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				app->audio->PlayFx(onClickFX);
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				SliderControl(mouseX, mouseY);
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				ret = NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			
		
			if (value >= 0)
				knobPos.x = bounds.x + outside.w - knob.w / 2;
			if (value >= 1)
				knobPos.x = bounds.x + outside.w - knob.w;

			isPlaying = false;

		}
	}

	return ret;
}

bool GuiSlidder::Draw(Render* render, SDL_Texture* tex)
{
	outside.w = inside.w * value;
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
		render->DrawTexture(tex, knobPos.x, knobPos.y, &knob);
		//draw

	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &inside);
		render->DrawTexture(tex, bounds.x, bounds.y, &outside);
		render->DrawTexture(tex, knobPos.x, knobPos.y, &knob);

	} break;
	case GuiControlState::PRESSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &inside);
		render->DrawTexture(tex, bounds.x, bounds.y, &outside);
		render->DrawTexture(tex, knobPos.x, knobPos.y, &knob);

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
	knobPos.x = mouseX - (knob.w / 2);
	LOG("Value %f", value);
	if (knobPos.x < bounds.x)
	{
		knobPos.x = bounds.x;
		value = minValue / 100;
	}
	if ((knobPos.x + knob.w) > (knobPos.x + bounds.w))
	{
		knobPos.x = (bounds.x + bounds.w) - knob.w;
		value = maxValue / 100;
	}
	value = (((maxValue - minValue) * (mouseX - (float)(bounds.x + knob.w / 2))) / (float)(bounds.w - knob.w) + minValue) / 100;
	// Limits
}