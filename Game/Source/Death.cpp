#include "Death.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "Fonts.h"
#include "UI.h"


Death::Death(bool startEnabled) : Module(startEnabled)
{
	name.Create("death");
	deathAnim.PushBack({ 0, 0, 32, 32 });
	deathAnim.PushBack({ 33, 0, 32, 32 });
	deathAnim.loop = true;
	deathAnim.speed = 0.1f;

	winAnim.PushBack({ 1, 1, 13, 11 });
	winAnim.PushBack({ 16, 1, 13, 11 });
	winAnim.PushBack({ 31, 1, 13, 11 });
	winAnim.PushBack({ 46, 1, 13, 11 });
	winAnim.PushBack({ 61, 1, 13, 11 });
	winAnim.loop = true;
	winAnim.speed = 0.1f;
}

// Destructor
Death::~Death()
{}

// Called before render is available
bool Death::Awake(pugi::xml_node& config)
{
	LOG("Loading Death");
	bool ret = true;
	folder.Create(config.child("folder").child_value());
	audioFile.Create(config.child("audio").child_value());
	return ret;
}

// Called before the first frame
bool Death::Start()
{
	// L03: DONE: Load map
	SString tmp("%s%s", folder.GetString(), "gameOver.png");
	SString tmp2("%s%s", folder.GetString(), "deathAnim.png");
	
	SString tmp3("%s%s", audioFile.GetString(), "music/end.wav");
	SString tmp4("%s%s", folder.GetString(), "win.png");
	SString tmp5("%s%s", folder.GetString(), "gem.png");
	app->currentScene = 3;
	app->audio->PlayMusic(tmp3.GetString());
	backgroundDeath = app->tex->Load(tmp.GetString());
	deathImg = app->tex->Load(tmp2.GetString());
	backgroundWin = app->tex->Load(tmp4.GetString());
	winImg = app->tex->Load(tmp5.GetString());
	app->tex->Enable();
	app->ui->Enable();
	app->render->camera.x = app->render->camera.y = 0;
	return true;
}

// Called each loop iteration
bool Death::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Death::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN  )
	{
		if (app->die)
		{
			app->hasLost = true;
			app->LoadGameRequest();
			if (app->currentScene == 1)
			{
				app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);

			}
			if (app->currentScene == 2)
			{
				app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene2);
			}
		}
		else
		{
			app->currentScene = 1;
			app->ui->ResetScore();
			app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
		}
	}
	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->camera.x = app->render->camera.y = 0;
	if (app->die)
	{
		deathAnim.Update();
		app->render->DrawTexture(backgroundDeath, 0, 0, NULL, 1.0f);
		SDL_Rect rect = deathAnim.GetCurrentFrame();
		app->render->DrawTexture(deathImg, 91, 181, &rect);
		app->render->DrawTexture(deathImg, 416, 181, &rect);
	}
	if (app->win_)
	{
		winAnim.Update();	
		app->render->DrawTexture(backgroundWin, 0, 0, NULL);
		SDL_Rect rect = winAnim.GetCurrentFrame();
		app->render->DrawTexture(winImg, 75, 250, &rect);
		app->render->DrawTexture(winImg, 475, 250, &rect);

	}

	return ret;
}

// Called each loop iteration
bool Death::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Death::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(backgroundDeath);
	app->tex->UnLoad(backgroundWin);
	app->tex->UnLoad(deathImg);
	app->ui->Disable();
	return true;
}