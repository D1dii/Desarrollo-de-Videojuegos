#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "Pathfinding.h"
#include "Map.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("boss");
}

Boss::~Boss()
{

}

void Boss::InitAnims()
{

}

bool Boss::Awake()
{



	return true;
}

bool Boss::Start()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	InitAnims();

	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	lifeBoss.x = position.x - 20;
	lifeBoss.y = position.y - 50;
	lifeBoss.w = 100;
	lifeBoss.h = 10;

	int enemy[8] = {
		-32, 0,
		16, 0,
		16, 48,
		-32, 48,
	};

	pendingDelete = false;

	pbody = app->physics->CreateChain(position.x + 8, position.y, enemy, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	

	currentAnim = &Walking;

	deathFx = app->audio->LoadFx(parameters.child("deathAudio").attribute("path").as_string());

	return true;
}

bool Boss::Update(float dt)
{

	// Draw Life of the Boss
	app->render->DrawRectangle(lifeBoss, 255, 0, 0, 255);

	// Draw Boss
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Boss::CleanUp()
{

	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::ATTACK:
		
		break;
	}
}