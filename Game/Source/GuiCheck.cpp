#include "GuiCheck.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"

GuiCheck::GuiCheck(uint32 id, SDL_Rect bounds,  const char* text, bool initState) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	//TODO Poner las animaciones

	canClick = true;
	drawBasic = false;

	checked = initState;

	pressed.PushBack({ 0,0,44,45 });
	focused.PushBack({ 46,0,44,45 });
	normal.PushBack({ 95,0,44,45 });
	xcheck.PushBack({ 148,0,20,45 });
		

	onClickFX = app->audio->LoadFx("Assets/audio/fx/onClickFX.wav");
	onHoverFX = app->audio->LoadFx("Assets/audio/fx/onHoverFX.wav");

}

GuiCheck::~GuiCheck()
{

}

bool GuiCheck::Update(float dt)
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
			if (isPlaying == false)
			{
				app->audio->PlayFx(onHoverFX);
				isPlaying = true;
			}	

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				app->audio->PlayFx(onClickFX);
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				checked = !checked;
				ret = NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			isPlaying = false;
		}
		
	}

	return ret;
}

bool GuiCheck::Draw(Render* render, SDL_Texture* tex)
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
		render->DrawTexture(tex, bounds.x, bounds.y, &normal.GetCurrentFrame());
		if (checked)
		{
			//draw
			render->DrawTexture(tex, bounds.x + 12, bounds.y, &xcheck.GetCurrentFrame());
		}
		else
		{
			//no draw
		}

	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &focused.GetCurrentFrame());
		if (checked)
		{
			//draw
			render->DrawTexture(tex, bounds.x + 12, bounds.y, &xcheck.GetCurrentFrame());
		}
	} break;
	case GuiControlState::PRESSED:
	{
		render->DrawTexture(tex, bounds.x, bounds.y, &pressed.GetCurrentFrame());
		if (checked)
		{
			//draw
			render->DrawTexture(tex, bounds.x + 12, bounds.y, &xcheck.GetCurrentFrame());
		}
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