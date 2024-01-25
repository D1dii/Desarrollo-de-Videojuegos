#include "Pozo.h"
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

Pozo::Pozo() : Entity(EntityType::POZO)
{
	name.Create("pozo");
}

Pozo::~Pozo()
{

}

void Pozo::InitAnims()
{
	
}

bool Pozo::Awake()
{



	//texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Pozo::Start()
{

	//initilize textures
	/*texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");*/

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	InitAnims();

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, width, height, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::POZO;
	pbody->body->SetGravityScale(0);

	

	return true;
}

bool Pozo::Update(float dt)
{


	return true;
}

bool Pozo::CleanUp()
{

	return true;
}

void Pozo::OnCollision(PhysBody* physA, PhysBody* physB)
{


}