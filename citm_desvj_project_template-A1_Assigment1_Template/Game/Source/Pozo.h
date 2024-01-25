#ifndef __POZO_H__
#define __POZO_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"

struct SDL_Texture;

class Pozo : public Entity
{
public:

	Pozo();

	~Pozo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void InitAnims();

	
public:

	const char* texturePath;

	SDL_Texture* texture = NULL;
	SDL_Texture* pathTest = NULL;

	int counterPath = 0;
	int timer = 0;

	int width;
	int height;

	SDL_Rect bound;

	bool debug = false;

	bool isDead = false;

	bool isShooting = false;

	int shootTimer = 0;

	bool isFacingLeft = true;

	uint deathFx;

private:
};

#endif // __ENEMY_H__
