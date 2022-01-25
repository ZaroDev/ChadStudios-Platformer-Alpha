#include "App.h"
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
	// L03: DONE: Load map
	SString tmp("%s%s", folder.GetString(), "intro.png");
	SString tmp2("%s%s", folder.GetString(), "logoAnim.png");
	SString tmp3("%s%s", folder.GetString(), "enter.png");
	SString tmp4("%s%s", audioFile.GetString(), "music/intro.wav");
	app->audio->PlayMusic(tmp4.GetString());

	background = app->tex->Load(tmp.GetString());
	logoImg = app->tex->Load(tmp2.GetString());
	enterImg = app->tex->Load(tmp3.GetString());

	//GUI
	uint x;
	uint y;
	app->win->GetWindowSize(x,y);
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Test1", { ((int)x / 2) - 700, (int)y / 10, 160, 40}, this);
	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Test2", { ((int)x / 2) -400, (int)y / 10, 160, 40 }, this);

	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
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

	app->render->DrawTexture(background, 0, 0, NULL);
	SDL_Rect rect = logoAnim.GetCurrentFrame();
	app->render->DrawTexture(logoImg, 0, 0, &rect);
	
	if ((frames / 60) % 2 == 0)
	{
		app->render->DrawTexture(enterImg, 152, 154, NULL);
	}

	//Draw GUI
	app->guiManager->Draw();

	return ret;
}

bool Intro::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 1)
		{
			LOG("Click on button 1");
		}

		if (control->id == 2)
		{
			LOG("Click on button 2");
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}


// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(logoImg);
	app->tex->UnLoad(enterImg);
	return true;
}