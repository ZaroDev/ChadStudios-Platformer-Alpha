#include "Death.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "FadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Death::Death(bool startEnabled) : Module(startEnabled)
{
	name.Create("death");
	DeathAnim.PushBack({ 0, 0, 32, 32 });
	DeathAnim.PushBack({ 33, 0, 32, 32 });
	DeathAnim.loop = true;
	DeathAnim.speed = 0.1f;
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
	SString tmp("%s%s", folder.GetString(), "intro.png");
	SString tmp2("%s%s", folder.GetString(), "deathAnim.png");
	
	SString tmp3("%s%s", audioFile.GetString(), "music/end.wav");
	app->audio->PlayMusic(tmp3.GetString());

	background = app->tex->Load(tmp.GetString());
	deathImg = app->tex->Load(tmp2.GetString());

	app->tex->Enable();


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
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fadeToBlack->MFadeToBlack(this, (Module*)app->scene);
	DeathAnim.Update();
	

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(background, 0, 0, NULL, 1.0f);
	SDL_Rect rect = DeathAnim.GetCurrentFrame();
	app->render->DrawTexture(deathImg, 20, 20, &rect);
	app->render->DrawTexture(deathImg, 20, 220, &rect);

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
	app->tex->UnLoad(background);
	app->tex->UnLoad(deathImg);

	return true;
}