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

	void InitAnims();
	
	Animation* currentAnim;
	Animation* lifeCurrentAnim;
	Animation* chargebarcurrentanim;

	Animation Idle;
	Animation IdleLeft;
	Animation RunLeft;
	Animation Run;
	Animation ChargeJump;
	Animation ChargeJumpLeft;
	Animation Jump;
	Animation Fall;
	Animation FallLeft;
	Animation attack;
	Animation attackLeft;
	Animation lifeAnim;
	Animation ChargeBar;
	Animation Stairs;
	Animation Death;
	Animation DeathRight;
	Animation WinAnim;

public:
	float speedx = 0.2f;
	float speedy = 1.0f;
	float jumpa = 0.06f;
	float power = 0;
	float speedPower = 1.0f;
	float jumpX = 0;
	float jumpY = 0;
	float triX = 200;
	float triY = 200;
	float savePos = position.x;
	float savePosY = position.y;
	int mouseX = 0;
	int mouseY = 0;
	float angle;
	int charge = 0;
	bool isCharging = false;

	int direction = 2;
	const char* texturePath;

	bool godMode = false;
	bool showBar = false;
	bool isScene = false;
	bool options = false;

	bool isDying;
	int timerDeadAnim;
	
	bool isFacingLeft;
	bool justFall = false;

	bool isAttacking = false;

	int attTimer;

	int fallTimer = 0;

	int lifes = 3;
	int lifeFrame = 0;
	bool canDmg = true;
	int dmgTimer = 0;

	int numMichelin = 0;
	bool canMichelin = true;
	int michelinTimer = 0;

	bool wonLevel = false;
	int winAnimTimer = 0;

	uint windowW, windowH;

	SDL_Texture* texture = NULL;
	SDL_Texture* hearts = NULL;
	SDL_Texture* chargebar = NULL;
	SDL_Texture* finalDoor = NULL;
	
	PhysBody* pbody;
	PhysBody* sword;
	int pickCoinFxId;

	PhysBody* escalera1 = nullptr;
	PhysBody* escalera2 = nullptr;
	PhysBody* winLevel = nullptr;
	
	SDL_Rect powerJump;
	SDL_Rect destinoRect;

	uint jumpFx;
	uint itemFx;
	uint michelinFx;
	uint victoryFx;

	pugi::xml_node configNode;

	enum jumpState 
	{
		JUMPING,
		POWER_JUMP,
		FLOOR,
		ESCALANDO
	};

	jumpState player = jumpState::FLOOR;
};

#endif // __PLAYER_H__