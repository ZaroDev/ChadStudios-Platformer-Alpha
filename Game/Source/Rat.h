#include "Enemy.h"

class Rat : public Enemy
{
public:
	Rat();
	~Rat();

	void Update();
	void ComputePath();
	void MoveToPlayer();
};
