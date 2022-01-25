#include "UI.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"
#include "Fonts.h"
#include "Player.h"

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
	if (app->currentScene == 1 || app->currentScene == 2)
	{
		heartAnim.Update();
		gemAnim.Update();
		abAnim.Update();
		if (app->entman->currentPlayer->GetHealth() == 3)
		{
			app->render->DrawTexture(heart, 115, 10, &heartAnim.GetCurrentFrame(), true);
		}
		if (app->entman->currentPlayer->GetHealth() >= 2)
		{
			app->render->DrawTexture(heart, 60, 10, &heartAnim.GetCurrentFrame(), true);
		}
		if (app->entman->currentPlayer->GetHealth() >= 1)
		{
			app->render->DrawTexture(heart, 5, 10, &heartAnim.GetCurrentFrame(), true);
		}
		SString tmp("%4d", score);
		SString tmp2("%d", (360 - app->entman->currentPlayer->abilityCD) / app->framesPerSecond);
		scoreMult = app->entman->currentPlayer->GetHealth();
		app->render->DrawTexture(gem, 1550, 10, &gemAnim.GetCurrentFrame(), true);
		app->fonts->BlitText(480, 5, font, tmp.GetString());
		if (app->entman->currentPlayer->abilityCD != 0)
		{
			app->fonts->BlitText(480, 270, font, tmp2.GetString());
		}
		else
		{
			app->render->DrawTexture(anim, 1420, 790, &abAnim.GetCurrentFrame(), true);
		}
	}
	else if (app->currentScene == 3)
	{
		SString tmp("score %4d", score);
		SString tmp2("score mult %i", scoreMult);
		SString tmp3("total score %4d", scoreMult * score);
		SString tmp4;
		counter++;
		if (highScore < scoreMult * score)
		{
			highScore = scoreMult * score;
			tmp4.Create("high score %4d", highScore);
		}
		else
		{
			tmp4.Create("new high score %4d", highScore);
		}
		if (counter <= 120)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(100, 50, font, tmp.GetString());
		}
		else
			app->fonts->BlitText(100, 50, font, tmp.GetString());

		if (counter <= 180)
		{
			if ((counter / 60) % 2 == 0)
			app->fonts->BlitText(100, 100, font, tmp2.GetString());
		}
		else
			app->fonts->BlitText(100, 100, font, tmp2.GetString());

		if (counter <= 240)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(100, 150, font, tmp3.GetString());
		}
		else
			app->fonts->BlitText(100, 150, font, tmp3.GetString());

		if (counter <= 300)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(100, 200, font, tmp4.GetString());
		}
		else
			app->fonts->BlitText(100, 200, font, tmp4.GetString());
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

void UI::AddScore(int score)
{
	this->score += score;
}

void UI::ResetScore()
{
	score = 0;
}

bool UI::LoadState(pugi::xml_node&data)
{
	score = data.child("score").attribute("value").as_int();
	highScore = data.child("highscore").attribute("value").as_int();

	return true;
}

bool UI::SaveState(pugi::xml_node&data)
{
	pugi::xml_node scr = data.append_child("score");
	scr.append_attribute("value").set_value(score);
	pugi::xml_node hscr = data.append_child("highscore");
	hscr.append_attribute("value").set_value(highScore);
	return true;
}
