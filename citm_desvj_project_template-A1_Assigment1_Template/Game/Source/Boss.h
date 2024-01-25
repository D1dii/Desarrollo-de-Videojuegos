#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();

	~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	Animation* currentAnim;
	Animation Walking;
	Animation Attacking;
	Animation Death;
	Animation WalkingRight;
	Animation AttackingRight;
	Animation DeathRight;

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	SDL_Texture* pathTest = NULL;

	int counterPath = 0;
	int timer = 0;

	int life = 100;

	SDL_Rect lifeBoss;

	SDL_Rect bound;

	bool debug = false;

	bool isDead = false;

	bool isShooting = false;

	int shootTimer = 0;

	bool isFacingLeft = true;

	uint deathFx;

private:
};

#endif // __BOSS_H__
