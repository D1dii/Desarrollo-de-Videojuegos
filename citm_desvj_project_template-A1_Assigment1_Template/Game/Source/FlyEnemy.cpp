#include "FlyEnemy.h"
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

FlyEnemy::FlyEnemy() : Entity(EntityType::FLY_ENEMY)
{
	name.Create("FlyEnemy");
}

FlyEnemy::~FlyEnemy()
{

}

void FlyEnemy::InitAnims()
{

}

bool FlyEnemy::Awake()
{

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool FlyEnemy::Start()
{

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	int enemy[8] = {
		0, 0,
		12, 0,
		12, 10,
		0, 10,
	};

	enemyCollider = app->physics->CreateCircle(position.x + 10, position.y + 15, 6, bodyType::DYNAMIC);
	enemyCollider->listener = this;
	enemyCollider->ctype = ColliderType::ENEMY;

	enemyCollider->body->SetGravityScale(0);

	return true;
}

bool FlyEnemy::Update(float dt)
{
	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (app->scene->GetPLayerPosition().x >= bound.x
		&& app->scene->GetPLayerPosition().x <= bound.x + bound.w
		&& app->scene->GetPLayerPosition().y >= bound.y
		&& app->scene->GetPLayerPosition().y <= bound.y + bound.h)
	{
		iPoint enemyPos = app->map->WorldToMap(position.x + 8, position.y + 16);
		iPoint playerPos = app->map->WorldToMap(app->scene->GetPLayerPosition().x + 8, app->scene->GetPLayerPosition().y);

		app->map->pathfinding->CreatePath(enemyPos, playerPos);

		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		if (debug) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(pathTest, pos.x, pos.y);
			}
		}
		

		if (path->Count() > 2 && app->map->pathfinding->CreatePath(enemyPos, playerPos) != -1) {

			iPoint pos = app->map->MapToWorld(path->At(2)->x, path->At(2)->y);

			triX = position.x - pos.x;
			triY = position.y - pos.y;

			enemyCollider->body->SetLinearVelocity(b2Vec2(-(triX / 10), -(triY / 10)));

			if (abs(enemyPos.x - playerPos.x) < 2) {
				enemyCollider->body->SetLinearVelocity(b2Vec2(0, 0));
				enemyCollider->body->SetLinearDamping(0);
			}
		}
	}

	position.x = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.x - 8);
	position.y = METERS_TO_PIXELS(enemyCollider->body->GetTransform().p.y - 8);

	bound.x = position.x - 120;
	bound.y = position.y - 60;
	bound.w = 240;
	bound.h = 120;

	if (debug) {
		app->render->DrawRectangle(bound, 0, 255, 0, 80);
	}
	
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool FlyEnemy::CleanUp()
{

	return true;
}

void FlyEnemy::OnCollision(PhysBody* physA, PhysBody* physB)
{

}