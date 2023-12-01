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

public:

	const char* texturePath;

	SDL_Texture* texture = NULL;

private:
};

#endif // __ENEMY_H__