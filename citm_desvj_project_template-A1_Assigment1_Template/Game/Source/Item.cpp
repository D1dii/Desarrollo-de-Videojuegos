#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

void Item::InitAnims()
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

bool Item::Awake() {

	

	return true;
}

bool Item::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	InitAnims();

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC, true);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;
	pbody->body->SetGravityScale(0);

	currentAnim = &Idle;

	return true;
}

bool Item::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	
	currentAnim->Update();

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		pendingDelete = true;
		
	}
}