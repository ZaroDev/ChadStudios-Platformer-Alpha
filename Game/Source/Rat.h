#include "Enemy.h"

class Rat : public Enemy
{
public:
	Rat(iPoint position_, Entity* target, int ID);

	void Update(float dt);
	void ComputePath(float dt);
	void MoveToPlayer(iPoint destination, float dt);



	int counterJump;
	bool canJump;
};
