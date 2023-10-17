#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

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

public:
	float speedx = 0.5f;
	float speedy = 1.0f;
	float jumpa = 0.06f;
	float power = 0;
	float speedPower = 1.4f;
	float savePos = position.x;
	float savePosY = position.y;
	int direction = 2;
	const char* texturePath;
	
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