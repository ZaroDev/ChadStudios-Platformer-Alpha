#include "Enemy.h"

class Eagle : public Enemy
{
public:
	Eagle();
	~Eagle();

	void Awake(pugi::xml_node& config);
	void Update();

};