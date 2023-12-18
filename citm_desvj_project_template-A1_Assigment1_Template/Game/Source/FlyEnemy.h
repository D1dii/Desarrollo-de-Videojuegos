#ifndef __FLYENEMY_H__
#define __FLYENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"

struct SDL_Texture;

class FlyEnemy : public Entity
{
public:

	FlyEnemy();

	~FlyEnemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	Animation* currentAnim;
	Animation Moving;
	Animation Attack;
	Animation MovingRight;
	Animation AttackRight;

	Animation Idle;

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	SDL_Texture* pathTest = NULL;

	int counterPath = 0;
	int timer = 0;

	float triX, triY;

	uint deathFx;

	SDL_Rect bound;

	bool debug = false;

	bool isDead = false;

	bool isFacingLeft = true;

	bool canExplode = true;
	bool isExploding = false;
	int explosionTimer = 0;

private:
};

#endif // __FLYENEMY_H__
