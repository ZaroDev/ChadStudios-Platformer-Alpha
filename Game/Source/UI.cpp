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

	heartAnim.PushBack({ 1, 1, 16, 16 });
	heartAnim.PushBack({ 20, 1, 16, 16 });
	heartAnim.PushBack({ 39, 1, 16, 16 });
	heartAnim.PushBack({ 78, 1, 16, 16 });
	heartAnim.PushBack({ 98, 1, 16, 16 });
	heartAnim.PushBack({ 1, 20, 16, 16 });
	heartAnim.PushBack({ 20, 20, 16, 16 });
	heartAnim.speed = 0.1f;
	heartAnim.loop = true;

	gemAnim.PushBack({ 1, 1, 13, 11 });
	gemAnim.PushBack({ 16, 1, 13, 11 });
	gemAnim.PushBack({ 31, 1, 13, 11 });
	gemAnim.PushBack({ 46, 1, 13, 11 });
	gemAnim.PushBack({ 61, 1, 13, 11 });
	gemAnim.loop = true;
	gemAnim.speed = 0.1f;

	return true;
}

bool UI::Start()
{
	SString tmp1("%s%s", folder.GetString(), "heart.png");
	SString tmp2("%s%s", folder.GetString(), "gem.png");

	heart = app->tex->Load(tmp1.GetString());
	gem = app->tex->Load(tmp2.GetString());

	return true;
}


bool UI::PostUpdate()
{
	heartAnim.Update();
	gemAnim.Update();
	if (app->player->lives == 3)
	{
		app->render->DrawTexture(heart, 115, 10, &heartAnim.GetCurrentFrame(), true);
	}
	if (app->player->lives >= 2)
	{
		app->render->DrawTexture(heart, 60, 10, &heartAnim.GetCurrentFrame(), true);
	}
	if (app->player->lives >= 1)
	{
		app->render->DrawTexture(heart, 5, 10, &heartAnim.GetCurrentFrame(), true);
	}

	app->render->DrawTexture(gem, 1550, 10, &gemAnim.GetCurrentFrame(), true);

	return true;
}



bool UI::CleanUp()
{
	app->tex->UnLoad(heart);
	return true;
}
