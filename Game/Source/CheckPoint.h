#include "Entity.h"
#include "Animation.h"

class CheckPoint : public Entity
{
public:
	CheckPoint();
	CheckPoint(iPoint position_);
	~CheckPoint();

	bool IsActivated();
	void Activate();

	void Update(float dt);
private:
	bool activated = false;
};