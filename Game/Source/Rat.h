#include "Enemy.h"

class Rat : public Enemy
{
public:
	Rat();
	~Rat();

	void Update(float dt);
	void ComputePath();
	void MoveToPlayer();
};
