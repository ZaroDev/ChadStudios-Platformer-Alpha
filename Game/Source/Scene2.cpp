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

#include "Defs.h"
#include "Log.h"

Scene2::Scene2(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
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
	LOG("%s", folder.GetString());
	return ret;
}

// Called before the first frame
bool Scene2::Start()
{
	SString tmp("%s%s", folder.GetString(), "background.png");
	SString tmp2("%s%s", folder.GetString(), "jungle.png");
	app->map->Load("map2.tmx");
	SString tmp3("%s%s", audioFile.GetString(), "music/lvl2.wav");
	app->audio->PlayMusic(tmp3.GetString());
	background = app->tex->Load(tmp.GetString());
	jungle = app->tex->Load(tmp2.GetString());
	LOG("%s", tmp.GetString());
	app->physics->Enable();
	app->player->Enable();
	app->audio->Enable();
	app->tex->Enable();
	app->map->Enable();

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


	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();
	if (app->player->debug)
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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(jungle);
	app->player->Disable();
	app->map->Disable();
	app->physics->Disable();
	return true;
}
