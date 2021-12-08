#include "CheckPoint.h"
#include "App.h"


CheckPoint::CheckPoint(bool startEnabled) : Module(startEnabled)
{
}

CheckPoint::~CheckPoint()
{

	checkpoints->Clear();
}

void CheckPoint::CreateCheckpoint(int x, int y)
{
	Flag* f = new Flag();

	f->body = app->physics->CreateRectangleSensor(x, y, 32, 53, bodyType::STATIC);
}
