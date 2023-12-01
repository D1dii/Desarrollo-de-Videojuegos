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

	return true;
}

bool Enemy::Update(float dt)
{

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