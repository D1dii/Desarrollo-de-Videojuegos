#include "Michelin.h"
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

Michelin::Michelin() : Entity(EntityType::MICHELIN)
{
	name.Create("michelin");
}

Michelin::~Michelin() {}

void Michelin::InitAnims()
{
	// Idle
	for (pugi::xml_node node = parameters.child("Idle").child("pushback"); node; node = node.next_sibling("pushback")) {
		Idle.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Idle.speed = parameters.child("Idle").attribute("animspeed").as_float();
	Idle.loop = parameters.child("Idle").attribute("loop").as_bool();
}

bool Michelin::Awake() {



	return true;
}

bool Michelin::Start() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	InitAnims();

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC, true);
	pbody->listener = this;
	pbody->ctype = ColliderType::MICHELIN;
	pbody->body->SetGravityScale(0);

	currentAnim = &Idle;

	return true;
}

bool Michelin::Update(float dt)
{
	int scale = app->win->GetScale();

	if (pos1)
	{
		position.x = 250;
		position.y = app->scene->player->position.y - 200;
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}
	else if (pos2)
	{
		position.x = 290;
		position.y = app->scene->player->position.y - 200;
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}
	else if (pos3)
	{
		position.x = 330;
		position.y = app->scene->player->position.y - 200;
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnim->Update();

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());

	return true;
}

bool Michelin::CleanUp()
{
	return true;
}

void Michelin::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		if (app->scene->player->numMichelin == 1)
		{
			pos1 = true;
		}
		else if (app->scene->player->numMichelin == 2)
		{
			pos2 = true;
		}
		else if (app->scene->player->numMichelin >= 3)
		{
			pos3 = true;
		}
		

	}
}