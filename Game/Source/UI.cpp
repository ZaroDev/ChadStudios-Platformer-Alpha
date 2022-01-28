#include "UI.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"
#include "Fonts.h"
#include "Player.h"
#include "Input.h"
#include "Window.h"
#include "GuiManager.h"
#include "Log.h"
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
	app->guiManager->Start();
	heart = app->tex->Load(tmp1.GetString());
	gem = app->tex->Load(tmp2.GetString());
	anim = app->tex->Load(tmp4.GetString());
	char lookUpTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789!.?   " };
	pausetex = app->tex->Load("Assets/textures/GUI/pauseMenu.png");
	settings = app->tex->Load("Assets/textures/GUI/settingsMenu.png");
	font = app->fonts->Load(tmp3.GetString(), lookUpTable, 7);
	//GUI
	app->guiManager->Start();
	btn1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::RESUME, 1, "Test1", {0,0, 154, 45 }, this);

	btn2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::SETTINGS, 2, "Test2", { 0,0, 154, 45 }, this);

	btn3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::BACKTOTILE, 3, "Test3", { 0,0, 154, 45 }, this);

	btn4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::CLOSE, 4, "Test4", { 0,0, 97, 42 }, this);
	btn5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GuiButtonType::EXIT, 5, "Test4", { 0,0, 97, 42 }, this);
	btn1->state = GuiControlState::NONE;
	btn2->state = GuiControlState::NONE;
	btn3->state = GuiControlState::NONE;
	btn4->state = GuiControlState::DISABLED;
	btn5->state = GuiControlState::NONE;
	settingsShow = false;
	pauseShow = false;


	return true;
}

bool UI::Update()
{
	
	return true;
}


bool UI::PostUpdate()
{
	if (app->currentScene == 1 || app->currentScene == 2)
	{
		seconds += app->dt / 1000;

		if (seconds >= 60)
		{
			seconds = 0;
			minutes++;
		}
		btn1->bounds.x = -app->render->camera.x / app->win->GetScale() + 200;
		btn1->bounds.y = -app->render->camera.y / app->win->GetScale() + 50;

		btn2->bounds.x = -app->render->camera.x / app->win->GetScale() + 200;
		btn2->bounds.y = -app->render->camera.y / app->win->GetScale() + 100;

		btn3->bounds.x = -app->render->camera.x / app->win->GetScale() + 200;
		btn3->bounds.y = -app->render->camera.y / app->win->GetScale() + 150;

		btn4->bounds.x = -app->render->camera.x / app->win->GetScale() + 200;
		btn4->bounds.y = -app->render->camera.y / app->win->GetScale() + 150;

		btn5->bounds.x = -app->render->camera.x / app->win->GetScale() + 225;
		btn5->bounds.y = -app->render->camera.y / app->win->GetScale() + 200;
	

		LOG("x%i y%i", app->render->camera.x, app->render->camera.y);
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
		SString tmp3("time %d.%.2f",minutes, seconds);
		scoreMult = app->entman->currentPlayer->GetHealth();
		app->render->DrawTexture(gem, 1550, 10, &gemAnim.GetCurrentFrame(), true);
		app->fonts->BlitText(480, 5, font, tmp.GetString());
		app->fonts->BlitText(400, 5, font, tmp3.GetString());
		if (app->entman->currentPlayer->abilityCD != 0)
		{
			app->fonts->BlitText(480, 270, font, tmp2.GetString());
		}
		else
		{
			app->render->DrawTexture(anim, 1420, 790, &abAnim.GetCurrentFrame(), true);
		}


		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			pauseShow = !pauseShow;

		if (pauseShow == true)
		{
			app->render->DrawTexture(pausetex, 500, 25, NULL, true);
			if (onPause == true)
			{
				btn1->state = GuiControlState::NORMAL;
				btn2->state = GuiControlState::NORMAL;
				btn3->state = GuiControlState::NORMAL;
				btn5->state = GuiControlState::NORMAL;
				onPause = false;
			}
		}
		else
		{
			btn1->state = GuiControlState::NONE;
			btn2->state = GuiControlState::NONE;
			btn3->state = GuiControlState::NONE;
			btn5->state = GuiControlState::NONE;
			onPause = true;
		}
		if (settingsShow == true)
		{
			app->render->DrawTexture(settings, 75, 25, NULL, true);
		}

		app->guiManager->Draw();
	}
	else if (app->currentScene == 3)
	{
		//SCORING SYSTEM
		SString tmp("score %4d", score);
		SString tmp2("score mult %i", scoreMult);
		SString tmp5("time %i.%f", minutes, seconds);
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


		//SCORE
		if (counter <= 120)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(225, 100, font, tmp.GetString());
		}
		else
			app->fonts->BlitText(225, 100, font, tmp.GetString());


		//SCORE MULT
		if (counter <= 240)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(225, 125, font, tmp2.GetString());
		}
		else
			app->fonts->BlitText(225, 125, font, tmp2.GetString());
		
		
		
		//TOTAL SCORE
		if (counter <= 360)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(225, 150, font, tmp3.GetString());
		}
		else
			app->fonts->BlitText(225, 150, font, tmp3.GetString());


		//HIGHSCORE
	
		if (counter <= 480)
		{
			if ((counter / 60) % 2 == 0)
				app->fonts->BlitText(225, 175, font, tmp4.GetString());
		}
		else
			app->fonts->BlitText(225, 175, font, tmp4.GetString());

	}
	return true;
}



bool UI::CleanUp()
{
	app->tex->UnLoad(heart);
	app->tex->UnLoad(gem);
	app->fonts->UnLoad(font);
	app->guiManager->CleanUp();
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
bool UI::OnGuiMouseClickEvent(GuiControl* control)
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
			if (app->currentScene == 1)
			{
				app->fadeToBlack->MFadeToBlack((Module*)app->scene, (Module*)app->intro);
			}
			if(app->currentScene == 2)
			{
				app->fadeToBlack->MFadeToBlack((Module*)app->scene2, (Module*)app->intro);
			}
		}

		if (control->id == 4)
		{
			settingsShow = false;
			btn1->state = GuiControlState::NORMAL;
			btn2->state = GuiControlState::NORMAL;
			btn3->state = GuiControlState::NORMAL;
			btn4->state = GuiControlState::DISABLED;
		}
		if (control->id == 5)
		{
			ret = false;
		}
	}
	//Other cases here

	default: break;
	}

	return ret;
}