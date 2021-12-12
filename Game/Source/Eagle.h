#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle();
	~Eagle();

	void Update(float dt);
	void ComputePath(float dt);
	void MoveToPlayer(iPoint destination, float dt);
	
	bool SaveState(pugi::xml_node&) const;
	bool LoadState(pugi::xml_node&);
};