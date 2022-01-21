#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle(iPoint position_, Entity* target_);


	void Update(float dt);
	void ComputePath(float dt);
	void MoveToPlayer(iPoint destination, float dt);
};