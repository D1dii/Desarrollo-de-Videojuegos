#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"


#define DEGTORAD 0.0174532925199432957f

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	
	Animation* currentAnim;

	Animation Idle;
	Animation Run;
	Animation ChargeJump;
	Animation Jump;

public:
	float speedx = 0.2f;
	float speedy = 1.0f;
	float jumpa = 0.06f;
	float power = 0;
	float speedPower = 1.0f;
	float jumpX = 0;
	float jumpY = 0;
	float triX = 0;
	float triY = 0;
	float savePos = position.x;
	float savePosY = position.y;
	int mouseX = 0;
	int mouseY = 0;
	float angle;

	int direction = 2;
	const char* texturePath;

	bool godMode = false;
	bool showBar = false;
	
	SDL_Texture* texture = NULL;
	
	PhysBody* pbody;
	int pickCoinFxId;
	
	SDL_Rect powerJump;
	

	enum jumpState 
	{
		JUMPING,
		POWER_JUMP,
		FLOOR
	};

	jumpState player = jumpState::FLOOR;
};

#endif // __PLAYER_H__