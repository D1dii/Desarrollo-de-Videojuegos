#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	Animation* currentAnim;

	Animation Idle;
	Animation Walking;
	Animation Shooting;

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	SDL_Texture* pathTest = NULL;

	int counterPath = 0;
	int timer = 0;

	PhysBody* enemyCollider;

	SDL_Rect bound;

	bool debug = false;

private:
};

#endif // __ENEMY_H__