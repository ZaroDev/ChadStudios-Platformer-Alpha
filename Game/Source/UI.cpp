#include "UI.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Fonts.h"

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

	abAnim.PushBack({ 5, 13, 20, 19 });
	abAnim.PushBack({ 31, 13, 20, 19 });
	abAnim.loop = true;
	abAnim.speed = 0.1f;

	return true;
}

bool UI::Start()
{
	SString tmp1("%s%s", folder.GetString(), "heart.png");
	SString tmp2("%s%s", folder.GetString(), "gem.png");
	SString tmp3("%s%s", folder.GetString(), "font.png");
	SString tmp4("%s%s", folder.GetString(), "abilityAnim.png");

	heart = app->tex->Load(tmp1.GetString());
	gem = app->tex->Load(tmp2.GetString());
	anim = app->tex->Load(tmp4.GetString());
	char lookUpTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789!.?   " };

	font = app->fonts->Load(tmp3.GetString(), lookUpTable, 7);

	return true;
}


bool UI::PostUpdate()
{
	heartAnim.Update();
	gemAnim.Update();
	abAnim.Update();
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
	SString tmp("%4d", app->player->score);
	SString tmp2("%d", (360 - app->player->abilityCD) / app->framesPerSecond);

	app->render->DrawTexture(gem, 1550, 10, &gemAnim.GetCurrentFrame(), true);
	app->fonts->BlitText(480, 5, font, tmp.GetString()); 
	if (app->player->abilityCD != 0)
	{
		app->fonts->BlitText(480, 270, font, tmp2.GetString());
	}
	else
	{
		
		app->render->DrawTexture(anim, 1420, 790, &abAnim.GetCurrentFrame(), true);
	}
	return true;
}



bool UI::CleanUp()
{
	app->tex->UnLoad(heart);
	app->tex->UnLoad(gem);
	app->fonts->UnLoad(font);
	return true;
}
