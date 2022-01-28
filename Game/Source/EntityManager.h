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
	void UpdateAll(float dt, bool canUpdate);
	//Util functions
	Entity* CreateEntity(EntityType type_, iPoint position_);
	void DestroyAllEntities();
	void DestroyEntity(Entity* entity);
	void SetPlayer(Player* player);
	void DrawPath(SDL_Texture* tex);

	void PlayJump();

public:
	Player* currentPlayer = nullptr;
private:
	int hitSFX;
	int playerHitSFX;
	int playerJumpSFX;
	int playerSuperJumpSFX;
	int checkpointSFX;
	int gemSFX;
	int cherrySFX;


	List<Entity*> entities;
	float accumulatedTime;
	float updateMsCycle;
	bool doLogic;

	SDL_Texture* playerTex;
	SDL_Texture* collectTex;
	SDL_Texture* checkTex;
	SDL_Texture* enemiesTex;
	SDL_Texture* path;

	SString folder;
	SString audio;

	int numRat = 0;
	int numEagle = 0;
	int numGem = 0;
	int numDoor = 0;
	int numCheckPoint = 0;
	int numCherry = 0;

	
};
