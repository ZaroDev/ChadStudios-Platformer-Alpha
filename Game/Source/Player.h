#ifndef __PLAYER_H__
#define __PLAYER_H__



#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Box2D/Box2D/Box2D.h"
#include "FadeToBlack.h"
#include "List.h"
struct SDL_Texture;


class Player : public Entity
{
public:
	Player(iPoint position_);

	void Update(float dt);
	void Use();
	void LoadAnims();

	void Initialize();

	int currentScene;
	bool grounded;
	bool god;
	bool useDownDash;
	int abilityCD;
	bool hurt;
private:
	//Jump sound
	int jumpSFX;
	int superJumpSFX;
	//Jump SFX folder path
	SString jumpSFXFile;
	//Variable for double jump
	int numJumps;
	//Velocity values
	float minVel;
	float maxVel;
	float jumpVel;
	//Sprite sheet folder path
	
	//Positions where the player should spawn in diferent levels
	int counterDash;

	int counter;
	//Player's b2 Body
	b2Body* c = nullptr;
	
	//Player texture
	SDL_Texture* tex;
	//Animations

	Animation idleAnimL;
	Animation idleAnimR;

	Animation runAnimL;
	Animation runAnimR;

	Animation jumpAnimL;
	Animation downAnimL;
	Animation jumpAnimR;
	Animation downAnimR;

	Animation hurtAnim;

	Animation abilityAnimL;
	Animation abilityAnimR;

};

#endif // !__PLAYER_H__