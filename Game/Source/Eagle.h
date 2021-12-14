#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle(iPoint position_);
	~Eagle();

	void Update(float dt);
	void ComputePath(float dt);
	void MoveToPlayer(iPoint destination, float dt);
};