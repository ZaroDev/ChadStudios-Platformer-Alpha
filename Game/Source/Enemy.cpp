#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

iPoint Enemy::GetPos()
{
	return pos;
}

void Enemy::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Enemy::Update()
{
}
