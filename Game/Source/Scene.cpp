#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "UI.h"
#include "Defs.h"
#include "Log.h"
#include "CheckPoint.h"
#include "Pathfinding.h"
#include "EntityManager.h"
Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("ing Scene");
	bool ret = true;
	folder.Create(config.child("folder").child_value());
	audioFile.Create(config.child("audio").child_value());
	winX = config.child("winX").attribute("value").as_int();
	LOG("%s", folder.GetString());
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	SString tmp("%s%s", folder.GetString(),"background.png");
	SString tmp2("%s%s", folder.GetString(), "jungle.png");
	SString tmp3("%s%s", audioFile.GetString(), "music/lvl1.wav");
	app->audio->PlayMusic(tmp3.GetString());
	background = app->tex->Load(tmp.GetString());
	jungle = app->tex->Load(tmp2.GetString());
	LOG("%s", tmp.GetString());
	app->physics->Enable();
	app->map->Enable();
	app->pathfinding->Enable();
	app->ui->Enable();
	Player* player = (Player*)app->entman->CreateEntity(PLAYER, iPoint{ 50, 800 });
	app->entman->SetPlayer(player);
	app->die = false;
	if (app->map->Load("map.tmx") == true)
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
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	

	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();
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
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || app->currentScene == 2)
	{
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene2);
		app->currentScene = 2;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN || app->die)
	{
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->death);
		app->die = true;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->death);
		app->win_ = true;
	}


	// Draw map
	app->render->DrawTexture(background, 0,0, NULL, false,0.75f);
	app->render->DrawTexture(jungle, 0, 284, NULL,false , 0.5f);
	app->map->Draw();
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(jungle);
	app->map->Unload();
	app->map->Disable();
	app->entman->DestroyAllEntities();
	app->physics->Disable();
	return true;
}
