#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	
	playTex = app->tex->Load("Assets/textures/GUI/play.png");
	continueTex = app->tex->Load("Assets/textures/GUI/continue.png");
	settingsTex = app->tex->Load("Assets/textures/GUI/settings.png");
	exitTex = app->tex->Load("Assets/textures/GUI/exit.png");
	creditsTex = app->tex->Load("Assets/textures/GUI/credits.png");
	resumeTex = app->tex->Load("Assets/textures/GUI/resume.png");
	bttTex = app->tex->Load("Assets/textures/GUI/backToTitle.png");
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, GuiButtonType btype, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L14: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* control = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:

		control = new GuiButton(id, bounds, btype, text);
		break;

		// More Gui Controls can go here

	default:
		break;
	}

	//Set the observer

	control->SetObserver(observer);
	//control->SetTexture(texture);

	// Created GuiControls are added to the list of controls
	if (control != nullptr) controls.Add(control);

	return control;
}

bool GuiManager::Update(float dt)
{
	bool ret = true;
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	ret = UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return ret;
}

bool GuiManager::UpdateAll(float dt, bool doLogic) {
	bool ret = true;
	if (doLogic) {

		ListItem<GuiControl*>* control = controls.start;

		while (control != nullptr)
		{
			ret = control->data->Update(dt);
			control = control->next;
		}

	}
	return ret;

}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		switch (control->data->buttonType)
		{
		
		default:
			break;

		case GuiButtonType::PLAY:
			control->data->Draw(app->render, playTex);
			break;		
		case GuiButtonType::CONTINUE:
			control->data->Draw(app->render, continueTex);
			break;		
		case GuiButtonType::SETTINGS:
			control->data->Draw(app->render, settingsTex);
			break;		
		case GuiButtonType::EXIT:
			control->data->Draw(app->render, exitTex);
			break;		
		case GuiButtonType::CREDITS:
			control->data->Draw(app->render, creditsTex);
			break;		
		case GuiButtonType::RESUME:
			control->data->Draw(app->render, resumeTex);
			break;		
		case GuiButtonType::BACKTOTILE:
			control->data->Draw(app->render, bttTex);
			break;		
		}



		control = control->next;
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}


	app->tex->UnLoad(playTex);
	app->tex->UnLoad(continueTex);
	app->tex->UnLoad(settingsTex);
	app->tex->UnLoad(exitTex);
	app->tex->UnLoad(creditsTex);
	app->tex->UnLoad(resumeTex);
	app->tex->UnLoad(bttTex);

	return true;
}