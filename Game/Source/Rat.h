#include "Enemy.h"

class Rat : public Enemy
{
public:
	Rat();
	~Rat();

	void Update(float dt);
	void ComputePath(float dt);
	void MoveToPlayer(iPoint destination, float dt);

	int counterJump;
	bool canJump;

	bool SaveState(pugi::xml_node&) const;
	bool LoadState(pugi::xml_node&);
};
