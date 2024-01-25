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
	//Walking
	for (pugi::xml_node node = parameters.child("Walking").child("pushback"); node; node = node.next_sibling("pushback")) {
		Walking.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Walking.speed = parameters.child("Walking").attribute("animspeed").as_float();
	Walking.loop = parameters.child("Walking").attribute("loop").as_bool();

	//Shooting
	for (pugi::xml_node node = parameters.child("Shooting").child("pushback"); node; node = node.next_sibling("pushback")) {
		Shooting.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Shooting.speed = parameters.child("Shooting").attribute("animspeed").as_float();
	Shooting.loop = parameters.child("Shooting").attribute("loop").as_bool();

	//WalkingRight
	for (pugi::xml_node node = parameters.child("WalkingRight").child("pushback"); node; node = node.next_sibling("pushback")) {
		WalkingRight.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	WalkingRight.speed = parameters.child("WalkingRight").attribute("animspeed").as_float();
	WalkingRight.loop = parameters.child("WalkingRight").attribute("loop").as_bool();

	//ShootingRight
	for (pugi::xml_node node = parameters.child("ShootingRight").child("pushback"); node; node = node.next_sibling("pushback")) {
		ShootingRight.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	ShootingRight.speed = parameters.child("ShootingRight").attribute("animspeed").as_float();
	ShootingRight.loop = parameters.child("ShootingRight").attribute("loop").as_bool();
}

bool Enemy::Awake()
{

	

	return true;
}

bool Enemy::Start()
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

	int enemy[8] = {
		-8, 0,
		16, 0,
		16, 24,
		-8, 24,
	};

	pendingDelete = false;

	pbody = app->physics->CreateChain(position.x + 8, position.y, enemy, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	shoot = app->physics->CreateCircle(position.x, position.y, 5, bodyType::DYNAMIC, true);
	shoot->listener = this;
	if (app->scene->isSaved) {
		shoot->ctype = ColliderType::ENEMY_ATTACK_SAVED;
	}
	else {
		shoot->ctype = ColliderType::ENEMY_ATTACK;
	}
	
	shoot->body->SetGravityScale(0);

	currentAnim = &Walking;

	deathFx = app->audio->LoadFx(parameters.child("deathAudio").attribute("path").as_string());

	return true;
}

bool Enemy::Update(float dt)
{

	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
	iPoint playerPos = app->map->WorldToMap(app->scene->GetPLayerPosition().x, app->scene->GetPLayerPosition().y);

	if (isDead == false && app->map->isMap1) {
		if (app->scene->GetPLayerPosition().x >= bound.x
			&& app->scene->GetPLayerPosition().x <= bound.x + bound.w
			&& app->scene->GetPLayerPosition().y >= bound.y
			&& app->scene->GetPLayerPosition().y <= bound.y + bound.h)
		{
			

			app->map->pathfinding->CreatePath(enemyPos, playerPos);

			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

			if (debug) {
				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(pathTest, pos.x, pos.y);
				}
			}


			if (path->Count() > 1 && app->map->pathfinding->CreatePath(enemyPos, playerPos) != -1) {

				if (enemyPos.x - playerPos.x < 0 && abs(enemyPos.x - playerPos.x) > 3) {
					pbody->body->SetLinearVelocity(b2Vec2(0.1 * dt, 0.2 * dt));
					isFacingLeft = false;
					currentAnim = &WalkingRight;
					isShooting = false;
				}
				else if (abs(enemyPos.x - playerPos.x) > 3) {
					pbody->body->SetLinearVelocity(b2Vec2(-0.1 * dt, 0.2 * dt));
					isFacingLeft = true;
					currentAnim = &Walking;
					isShooting = false;
				}
				else if (abs(enemyPos.x - playerPos.x) < 3) {
					pbody->body->SetLinearVelocity(b2Vec2(0, 0.2 * dt));
					pbody->body->SetLinearDamping(0);

					if (isFacingLeft) {
						currentAnim = &Shooting;
					}
					else {
						currentAnim = &ShootingRight;
					}
					
					if(shootTimer >= 30) isShooting = true;
					
					if (shootTimer >= 60) {
						shoot->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x + 15)), PIXEL_TO_METERS((float32)(position.y + 18)) }, 0);
						isShooting = false;
						shootTimer = 0;
					}
					shootTimer++;
				}
			}
		}
		else {
			if (isFacingLeft) {
				currentAnim = &Walking;
			}
			else if (!isFacingLeft) {
				currentAnim = &WalkingRight;
			}
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 12);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 6);

		if (isShooting == false) {
			shoot->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x + 15)), PIXEL_TO_METERS((float32)(position.y + 18)) }, 0);
		}
		else if (isShooting) {
			
			if (isFacingLeft) {
				shoot->body->SetLinearVelocity(b2Vec2(-1.5f, 0));
			}
			else if (isFacingLeft == false) {
				shoot->body->SetLinearVelocity(b2Vec2(1.5f, 0));
			}
			
		}

		bound.x = position.x - 120;
		bound.y = position.y - 60;
		bound.w = 240;
		bound.h = 120;

		if (app->physics->debug) {
			app->render->DrawRectangle(bound, 0, 255, 0, 80);
		}

		currentAnim->Update();
		app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	}
	
	
	if (enemyPos.x - playerPos.x < 0 && abs(enemyPos.x - playerPos.x) > 3) {
		isFacingLeft = false;
		currentAnim = &WalkingRight;
	}
	else if (abs(enemyPos.x - playerPos.x) > 3) {
		isFacingLeft = true;
		currentAnim = &Walking;
	}

	return true;
}

bool Enemy::CleanUp()
{

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::ATTACK:
		isDead = true;
		pendingDelete = true;
		app->audio->PlayFx(deathFx);
		break;
	}
}