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
#include "GuiManager.h"

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
	startX = config.child("startX").attribute("value").as_int();
	startY = config.child("startY").attribute("value").as_int();
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

	//GUI
	pausetex = app->tex->Load("Assets/textures/GUI/pauseMenu.png");
	settings = app->tex->Load("Assets/textures/GUI/settingsMenu.png");
	//

	app->currentScene = 1;
	app->physics->Enable();
	app->map->Enable();
	app->pathfinding->Enable();
	app->ui->Enable();
	Player* player = (Player*)app->entman->CreateEntity(PLAYER, iPoint{ startX, startY });
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

	app->entman->Enable();

	if (app->hasLoaded && app->canContinue)
	{
		app->LoadGameRequest();
		app->canContinue = false;
	}

	//GUI
	uint x;
	uint y;
	app->win->GetWindowSize(x, y);
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::RESUME, 1, "Test1", { ((int)x / 2) - 600, (int)y / 10, 97, 42 }, this);

	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::SETTINGS, 2, "Test2", { ((int)x / 2) - 625, ((int)y / 10) + 50, 154, 45 }, this);

	btn3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::BACKTOTILE, 3, "Test3", { ((int)x / 2) - 625, ((int)y / 10) + 100, 154, 45 }, this);

	btn4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::CLOSE, 4, "Test4", { ((int)x / 2) - 350, ((int)y / 10) - 65, 97, 42 }, this);

	btn4->state = GuiControlState::DISABLED;
	settingsShow = false;
	pauseShow = false;

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
	LOG("%i\n", app->currentScene);

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

	app->guiManager->Draw();
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		pauseShow = true;

	if (pauseShow == true)
	{
		app->render->DrawTexture(pausetex, 75, 25, NULL);
		btn1->state = GuiControlState::NORMAL;
		btn2->state = GuiControlState::NORMAL;
		btn3->state = GuiControlState::NORMAL;
	}
	if (settingsShow == true)
	{
		app->render->DrawTexture(settings, 75, 25, NULL);
	}

	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	bool ret = true;
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 1)
		{
			pauseShow = false;
			btn1->state = GuiControlState::DISABLED;
			btn2->state = GuiControlState::DISABLED;
			btn3->state = GuiControlState::DISABLED;
		}

		if (control->id == 2)
		{
			settingsShow = true;
			btn1->state = GuiControlState::DISABLED;
			btn2->state = GuiControlState::DISABLED;
			btn3->state = GuiControlState::DISABLED;
			btn4->state = GuiControlState::NORMAL;
		}

		if (control->id == 3)
		{
			app->fadeToBlack->MFadeToBlack(this, (Module*)app->intro);
		}

		if (control->id == 4)
		{
			settingsShow = false;
			btn1->state = GuiControlState::NORMAL;
			btn2->state = GuiControlState::NORMAL;
			btn3->state = GuiControlState::NORMAL;
			btn4->state = GuiControlState::DISABLED;
		}
	}
	//Other cases here

	default: break;
	}

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
