#ifndef __MICHELIN_H__
#define __MICHELIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "../Animation.h"

struct SDL_Texture;

class Michelin : public Entity
{
public:

	Michelin();
	virtual ~Michelin();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool CleanUp();

	void InitAnims();

public:

	bool isPicked = false;

	Animation* currentAnim;

	Animation Idle;

	bool pos1 = false;
	bool pos2 = false;
	bool pos3 = false;



private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __ITEM_H__
