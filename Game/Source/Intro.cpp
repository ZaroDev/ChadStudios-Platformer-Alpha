#include "App.h"
#include "EntityManager.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

Intro::Intro(bool startEnabled) : Module(startEnabled)
{
	name.Create("intro");
	logoAnim.PushBack({0, 0, 534, 300});
	logoAnim.PushBack({ 534, 0, 534, 300 });
	logoAnim.PushBack({ 534*2, 0, 534, 300 });
	logoAnim.PushBack({ 534*3, 0, 534, 300 });
	logoAnim.PushBack({ 534*4, 0, 534, 300 });
	logoAnim.PushBack({ 534 * 3, 0, 534, 300 });
	logoAnim.PushBack({ 534 * 2, 0, 534, 300 });
	logoAnim.PushBack({ 534, 0, 534, 300 });
	logoAnim.PushBack({ 0, 0, 534, 300 });
	logoAnim.loop = true;
	logoAnim.speed = 0.07f;
}

// Destructor
Intro::~Intro()
{}

// Called before render is available
bool Intro::Awake(pugi::xml_node& config)
{
	LOG("Loading Intro");
	bool ret = true;
	folder.Create(config.child("folder").child_value()); 
	audioFile.Create(config.child("audio").child_value());
	return ret;
}

// Called before the first frame
bool Intro::Start()
{
	app->guiManager->Start();
	// L03: DONE: Load map
	SString tmp("%s%s", folder.GetString(), "intro.png");
	SString tmp2("%s%s", folder.GetString(), "logoAnim.png");
	//SString tmp3("%s%s", folder.GetString(), "creditsMenu.png");
	//SString tmp5("%s%s", folder.GetString(), "settingsMenu.png");
	SString tmp4("%s%s", audioFile.GetString(), "music/intro.wav");
	app->audio->PlayMusic(tmp4.GetString());
	app->currentScene = 0;
	background = app->tex->Load(tmp.GetString());
	logoImg = app->tex->Load(tmp2.GetString());
	credits = app->tex->Load("Assets/textures/GUI/creditsMenu.png");
	settings = app->tex->Load("Assets/textures/GUI/settingsMenu.png");

	load = true;
	//GUI
	uint x;
	uint y;
	app->win->GetWindowSize(x,y);
	
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::PLAY, 1, "Test1", { ((int)x / 2) - 600, (int)y / 10, 97, 42}, this);

	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::CONTINUE, 2, "Test2", { ((int)x / 2) - 625, ((int)y / 10) + 50, 154, 45 }, this);
	
	btn3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::SETTINGS,3, "Test3", { ((int)x / 2) - 625, ((int)y / 10) + 100, 154, 45 }, this);

	btn4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::EXIT, 4, "Test4", { ((int)x / 2) - 600, ((int)y / 10) + 150, 97, 42 }, this);

	btn5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::CREDITS, 5, "Test5", { ((int)x / 2) - 400, ((int)y / 10) + 150, 97, 42 }, this);
	
	btn6 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::CLOSE, 6, "Test6", { ((int)x / 2) - 350, ((int)y / 10) - 65, 97, 42 }, this);

	check1 = (GuiCheck*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GuiButtonType::NONE, 7, "Check1", { ((int)x / 2) - 350, ((int)y / 10) - 65, 97, 42 }, this);

	slid1 = (GuiSlidder*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, GuiButtonType::NONE, 8, "sld1", { ((int)x / 2) - 500, ((int)y / 10), 210, 38 }, this);


	btn6->state = GuiControlState::DISABLED;



	settingsShow = false;
	creditShow = false;
	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
	LOG("Cam x%i y%i", app->render->camera.x, app->render->camera.y);
	if (load)
	{
		app->LoadGameRequest();
		load = false;
	}
	if (app->hasLoaded && !creditShow && !settingsShow)
	{
		btn2->state = GuiControlState::NORMAL;
	}
	LOG("%i", app->hasLoaded);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	return true;
}

// Called each loop iteration
bool Intro::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();
	
	logoAnim.Update();
	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{
	bool ret = true;
	frames++;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	bool draw = app->render->DrawTexture(background, 0, 0, NULL);

	SDL_Rect rect = logoAnim.GetCurrentFrame();
	app->render->DrawTexture(logoImg, 0, 0, &rect);

	//Draw GUI
	if (creditShow == true)
	{
		app->render->DrawTexture(credits, 75, 25, NULL);
	}
	if (settingsShow == true)
	{
		app->render->DrawTexture(settings, 75, 25, NULL);
	}

	app->guiManager->Draw();

	
	return ret;
}

bool Intro::OnGuiMouseClickEvent(GuiControl* control)
{
	bool ret = true;
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 1)
		{
			app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
		}

		if (control->id == 2)
		{
			//checkear archivo cargado
			app->canContinue = true;
			if(app->currentScene == 1)
				app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
			else
				app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene2);
		}
		
		if (control->id == 3)
		{
			settingsShow = true;
			btn1->state = GuiControlState::NONE;
			btn2->state = GuiControlState::NONE;
			btn3->state = GuiControlState::NONE;
			btn4->state = GuiControlState::NONE;
			btn5->state = GuiControlState::NONE;
			btn6->state = GuiControlState::NORMAL;
		}
		
		if (control->id == 4)
		{
			ret = false;
		}
		
		if (control->id == 5)
		{
			creditShow = true;
			btn1->state = GuiControlState::NONE;
			btn2->state = GuiControlState::NONE;
			btn3->state = GuiControlState::NONE;
			btn4->state = GuiControlState::NONE;
			btn5->state = GuiControlState::NONE;
			btn6->state = GuiControlState::NORMAL;
		}

		if (control->id == 6)
		{
			creditShow = false;
			settingsShow = false;
			btn1->state = GuiControlState::NORMAL;
			btn2->state = GuiControlState::NORMAL;
			btn3->state = GuiControlState::NORMAL;
			btn4->state = GuiControlState::NORMAL;
			btn5->state = GuiControlState::NORMAL;
			btn6->state = GuiControlState::NONE;
		}
	}
	//Other cases here

	default: break;
	}

	return ret;
}

bool Intro::SaveState(pugi::xml_node& data) const
{
	return true;
}

bool Intro::LoadState(pugi::xml_node& data)
{
	return true;
}


// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(logoImg);
	app->guiManager->CleanUp();
	return true;
}