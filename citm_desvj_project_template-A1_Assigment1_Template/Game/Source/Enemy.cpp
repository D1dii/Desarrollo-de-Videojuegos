#include "Enemy.h"
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

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");
}

Enemy::~Enemy()
{

}

void Enemy::InitAnims()
{

}

bool Enemy::Awake()
{

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Enemy::Start()
{

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");

	int enemy[8] = {
		0, 0,
		12, 0,
		12, 16,
		0, 16,
	};

	enemyCollider = app->physics->CreateChain(position.x + 8, position.y, enemy, 8, bodyType::DYNAMIC);
	enemyCollider->listener = this;
	enemyCollider->ctype = ColliderType::ENEMY;

	return true;
}

bool Enemy::Update(float dt)
{

	iPoint origin = iPoint(10, 136);
	iPoint destination = iPoint(20, 134);

	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
	iPoint playerPos = app->map->WorldToMap(app->scene->GetPLayerPosition().x, app->scene->GetPLayerPosition().y);

	app->map->pathfinding->CreatePath(enemyPos, playerPos);

	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();



	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(pathTest, pos.x, pos.y);
	}

	if (timer >= 60) {
		iPoint pos = app->map->MapToWorld(path->At(0)->x, path->At(0)->y);
		enemyCollider->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0);
		timer = 0;
	}
	timer++;

	position.x = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.x - 5);
	position.y = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.y - 4);

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Enemy::CleanUp()
{

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{

}