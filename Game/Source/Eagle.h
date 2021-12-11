#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle();
	~Eagle();

	void Update();
	void ComputePath();
	void MoveToPlayer();
};