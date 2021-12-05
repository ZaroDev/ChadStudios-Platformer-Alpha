#include "UI.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"

UI::UI(bool startEnabled) : Module(startEnabled)
{
	name.Create("ui");
}

UI::~UI()
{
}

bool UI::Awake(pugi::xml_node& config)
{
	folder.Create(config.child("folder").child_value());

	return true;
}

bool UI::Start()
{
	//heart = app->tex->Load(folder.GetString());
	heart = app->tex->Load("Assets/textures/UI/heart.png");



	return true;
}


bool UI::PostUpdate()
{

	if (app->player->lives == 3)
	{
		app->render->DrawTexture(heart, 115, 10, NULL, true);
	}
	if (app->player->lives >= 2)
	{
		app->render->DrawTexture(heart, 60, 10, NULL, true);
	}
	if (app->player->lives >= 1)
	{
		app->render->DrawTexture(heart, 5, 10, NULL, true);
	}

	return true;
}



bool UI::CleanUp()
{
	app->tex->UnLoad(heart);
	return true;
}
