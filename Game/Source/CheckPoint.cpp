#include "CheckPoint.h"
#include "Textures.h"
#include "App.h"
#include "Render.h"
#include "Audio.h"

CheckPoint::CheckPoint(bool startEnabled) : Module(startEnabled)
{
	name.Create("check");
}

CheckPoint::~CheckPoint()
{

	checkpoints.Clear();
}

bool CheckPoint::Awake(pugi::xml_node& config)
{
	folder.Create(config.child("folder").child_value());
	sfx.Create(config.child("sfx").child_value());

	return true;
}

bool CheckPoint::Start()
{
	SString tmp("%s%s", folder.GetString(), "checkpoint.png");
	SString tmp2("%s%s", sfx.GetString(), "check.wav");
	tex = app->tex->Load(tmp.GetString());
	SFX = app->audio->LoadFx(tmp2.GetString());
	return true;
}

bool CheckPoint::Update(float dt)
{
	ListItem<Flag*>* f = checkpoints.start;

	while (f != NULL)
	{
		if (f->data->isActive == false)
		{
			f->data->flagAnim.Update();
		}
		app->render->DrawTexture(tex, f->data->pos.x, f->data->pos.y, &f->data->flagAnim.GetCurrentFrame());

		f = f->next;
	}

	return true;
}

bool CheckPoint::CleanUp()
{
	for (ListItem<Flag*>* c = checkpoints.start; c != NULL; c = c->next)
	{
		app->physics->world->DestroyBody(c->data->body->body);
	}
	app->tex->UnLoad(tex);
	checkpoints.Clear();
	return true;
}

void CheckPoint::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	ListItem<Flag*>* f = checkpoints.start;

	while (f != NULL)
	{
		if (bodyA == f->data->body && f->data->isActive == true  && bodyB->listener == (Module*)app->player)
		{
			f->data->isActive = false;
			
			app->audio->PlayFx(SFX);
			app->SaveGameRequest();
		}
		
		f = f->next;
	}
	
}

void CheckPoint::CreateCheckpoint(int x, int y)
{
	Flag* f = new Flag();

	f->body = app->physics->CreateRectangleSensor(x, y, 32, 53, bodyType::STATIC);
	f->pos.x = x - 16;
	f->pos.y = y - 27;
	f->body->listener = this;
	f->flagAnim.PushBack({ 64, 0, 32, 53 });
	f->flagAnim.PushBack({ 32, 0, 32, 53 });
	f->flagAnim.PushBack({ 0, 0, 32, 53 });
	f->flagAnim.speed = 0.1f;
	f->flagAnim.loop = false;

	checkpoints.Add(f);
}
