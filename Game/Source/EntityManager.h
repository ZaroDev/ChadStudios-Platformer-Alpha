#include "Entity.h"
#include "Module.h"
#include "List.h"
class Player;
class EntityManager : public Module
{
public:
	EntityManager(bool startEnabled);
	~EntityManager();
	//Module Functions
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	
	bool SaveState(pugi::xml_node& data) const;
	bool LoadState(pugi::xml_node& data);
	//Updates all the entities
	void UpdateAll(float dt, bool doLogic);
	//Util functions
	Entity* CreateEntity(EntityType type_, iPoint position_);
	void DestroyAllEntities();
	void DestroyEntity(Entity* entity);
	void SetPlayer(Player* player);


public:
	Player* currentPlayer = nullptr;
private:
	int hitSFX;
	int playerHit;
	List<Entity*> entities;
	float accumulatedTime;
	float updateMsCycle;
	bool doLogic;
	SDL_Texture* playerTex;
	SDL_Texture* collectTex;
	SDL_Texture* checkTex;
	SDL_Texture* enemiesTex;
	SString folder;
};
