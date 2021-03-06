#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene2.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Pathfinding.h"
#include "CheckPoint.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "UI.h"

Scene2::Scene2(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	folder.Create(config.child("folder").child_value());
	audioFile.Create(config.child("audio").child_value());
	startX = config.child("startX").attribute("value").as_int();
	startY = config.child("startY").attribute("value").as_int();
	LOG("%s", folder.GetString());
	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	SString tmp("%s%s", folder.GetString(), "background.png");
	SString tmp2("%s%s", folder.GetString(), "jungle.png");
	SString tmp3("%s%s", audioFile.GetString(), "music/lvl2.wav");
	app->audio->PlayMusic(tmp3.GetString());
	background = app->tex->Load(tmp.GetString());
	jungle = app->tex->Load(tmp2.GetString());
	LOG("%s", tmp.GetString());
	app->physics->Enable();
	app->map->Enable();
	app->audio->Enable();
	app->ui->Enable();
	app->currentScene = 2;
	app->entman->Enable();
	Player* player = (Player*)app->entman->CreateEntity(PLAYER, iPoint{ startX, startY });
	app->entman->SetPlayer(player);
	app->die = false;
	if (app->map->Load("map2.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &data))
		{
			app->pathfinding->SetMap(w, h, data);
		}

		RELEASE_ARRAY(data);
	}

	if (app->hasLost)
	{
		app->LoadGameRequest();
	}
	if (app->hasLoaded && app->canContinue)
	{
		app->LoadGameRequest();
		app->canContinue = false;
	}
	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{


	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN || app->die)
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->death);

	if (app->debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += 30;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= 30;

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += 30;

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= 30;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || app->currentScene == 1)
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
	
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || app->currentScene == 3)
	{
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->death);
		app->win_ = true;
	}

	// Draw map
	app->render->DrawTexture(background, 0, 0, NULL, 0.75f);
	app->render->DrawTexture(jungle, 0, 284, NULL, 0.5f);
	app->map->Draw();



	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(jungle);
	app->entman->DestroyAllEntities();
	app->map->Unload();
	app->map->Disable();

	app->ui->Disable();
	app->physics->Disable();
	return true;
}
