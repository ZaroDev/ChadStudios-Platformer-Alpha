#include "Enemies.h"
#include "Eagle.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
Enemies::Enemies(bool startEnabled) : Module(startEnabled)
{
}

Enemies::~Enemies()
{
}

bool Enemies::Awake(pugi::xml_node&config)
{
	return true;
	folder.Create(config.child("folder").child_value());
	
}

bool Enemies::Start()
{
	return true;
	tex = app->tex->Load(folder.GetString());
}

bool Enemies::Update()
{
	//for (ListItem<Enemy*>* e = enemies->start; e != NULL; e = e->next)
	//{
	//	e->data->Update();
	//}
	return true;
}

bool Enemies::PostUpdate()
{
	/*for (ListItem<Enemy*>* e = enemies->start; e != NULL; e = e->next)
	{
		app->render->DrawTexture(e->data->tex, e->data->GetPos().x, e->data->GetPos().y, &e->data->anim.GetCurrentFrame());
	}*/
	return true;
}

bool Enemies::CleanUp()
{
	enemies.Clear();
	return true;
}

void Enemies::CreateEnemy(EnemyType type, float x, float y)
{
	/*switch (type)
	{
	case EAGLE:
	{
		Eagle* e = new Eagle();
		e->SetPos(x, y);
		enemies->Add(e);
		break;
	}
	case RAT:
		break;
	case MAX:
		break;
	default:
		break;
	}*/
}


