#include "Eagle.h"

Eagle::Eagle() : Enemy()
{
}

Eagle::~Eagle()
{
}

void Eagle::Awake(pugi::xml_node& config)
{

	anim.PushBack({ 0,28,38,42 });
	anim.PushBack({ 40,31,40,40 });
	anim.PushBack({ 80,40,32,30 });
	anim.PushBack({ 117,40,40,30 });
	anim.speed = 0.1f;
	anim.loop = true;
}

void Eagle::Update()
{

}
