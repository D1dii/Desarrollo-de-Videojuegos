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
#include "ScreenEnd.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("boss");
}

Boss::~Boss()
{

}

void Boss::InitAnims()
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

	//WalkingRight
	for (pugi::xml_node node = parameters.child("WalkingRight").child("pushback"); node; node = node.next_sibling("pushback")) {
		WalkingRight.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	WalkingRight.speed = parameters.child("WalkingRight").attribute("animspeed").as_float();
	WalkingRight.loop = parameters.child("WalkingRight").attribute("loop").as_bool();

	//Death
	for (pugi::xml_node node = parameters.child("Death").child("pushback"); node; node = node.next_sibling("pushback")) {
		Death.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Death.speed = parameters.child("Death").attribute("animspeed").as_float();
	Death.loop = parameters.child("Death").attribute("loop").as_bool();

	//DeathRight
	for (pugi::xml_node node = parameters.child("DeathRight").child("pushback"); node; node = node.next_sibling("pushback")) {
		DeathRight.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	DeathRight.speed = parameters.child("DeathRight").attribute("animspeed").as_float();
	DeathRight.loop = parameters.child("DeathRight").attribute("loop").as_bool();

	//Attacking
	for (pugi::xml_node node = parameters.child("Attacking").child("pushback"); node; node = node.next_sibling("pushback")) {
		Attacking.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Attacking.speed = parameters.child("Attacking").attribute("animspeed").as_float();
	Attacking.loop = parameters.child("Attacking").attribute("loop").as_bool();

	//AttackingRight
	for (pugi::xml_node node = parameters.child("AttackingRight").child("pushback"); node; node = node.next_sibling("pushback")) {
		AttackingRight.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	AttackingRight.speed = parameters.child("AttackingRight").attribute("animspeed").as_float();
	AttackingRight.loop = parameters.child("AttackingRight").attribute("loop").as_bool();

	//HP
	for (pugi::xml_node node = parameters.child("HP").child("pushback"); node; node = node.next_sibling("pushback")) {
		HP.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	HP.speed = parameters.child("HP").attribute("animspeed").as_float();
	HP.loop = parameters.child("HP").attribute("loop").as_bool();

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

	currentAnim = &Walking;

	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);
	pathTest = app->tex->Load("Assets/Textures/testPathTile.png");
	lifeBoss = app->tex->Load("Assets/Textures/boss hp.png");

	bound.x = position.x - 300;
	bound.y = position.y - 60;
	bound.w = 500;
	bound.h = 120;

	int enemy[8] = {
		-32, 0,
		40, 0,
		40, 64,
		-32, 64,
	};

	pendingDelete = false;

	pbody = app->physics->CreateChain(position.x + 8, position.y, enemy, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	shoot = app->physics->CreateRectangleSensor(position.x + 8, position.y + 32, 120, 16,  bodyType::DYNAMIC);
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

bool Boss::Update(float dt)
{
	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	int scale = app->win->GetScale();

	// Get Positions for Path
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
	iPoint playerPos = app->map->WorldToMap(app->scene->GetPLayerPosition().x, app->scene->GetPLayerPosition().y);
	
	if (isDead == false && !app->map->isMap1) {
		if (app->scene->GetPLayerPosition().x >= bound.x
			&& app->scene->GetPLayerPosition().x <= bound.x + bound.w
			&& app->scene->GetPLayerPosition().y >= bound.y
			&& app->scene->GetPLayerPosition().y <= bound.y + bound.h)
		{

			// Create Pathfinding
			app->map->pathfinding->CreatePath(enemyPos, playerPos);

			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

			// Draw Path
			if (debug) {
				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(pathTest, pos.x, pos.y);
				}
			}

			// Combat of the Boss
			if (path->Count() > 1 && app->map->pathfinding->CreatePath(enemyPos, playerPos) != -1) {

				if (enemyPos.x - playerPos.x < 0 && abs(enemyPos.x - playerPos.x) > 3) {
					pbody->body->SetLinearVelocity(b2Vec2(0.1 * dt, 0.2 * dt));
					if (!isAttacking)
					{
						isFacingLeft = false;
						currentAnim = &WalkingRight;
					}
					
					
				}
				else if (abs(enemyPos.x - playerPos.x) > 3) {
					pbody->body->SetLinearVelocity(b2Vec2(-0.1 * dt, 0.2 * dt));
					if (!isAttacking)
					{
						isFacingLeft = true;
						currentAnim = &Walking;
					}
					
					
					
				}
				else if (abs(enemyPos.x - playerPos.x) < 3) {
					pbody->body->SetLinearVelocity(b2Vec2(0, 0.2 * dt));
					pbody->body->SetLinearDamping(0);

					if (isFacingLeft) {
						currentAnim = &Attacking;
					}
					else {
						currentAnim = &AttackingRight;
					}

					if (attackTimer >= 30) isAttacking = true;

					attackTimer++;

				}
			}
			
		}
		else {
			if (isFacingLeft) {
				currentAnim = &Walking;
				isAttacking = false;
				Attacking.SetCurrentFrame(0);
				AttackingRight.SetCurrentFrame(0);
				
			}
			else if (!isFacingLeft) {
				currentAnim = &WalkingRight;
				isAttacking = false;
				Attacking.SetCurrentFrame(0);
				AttackingRight.SetCurrentFrame(0);
			}
		}

		// Jump Attack
		if (isAttacking)
		{
			pbody->body->SetLinearVelocity(b2Vec2(0, jumpForce));
			jumpForce += 0.1f;
			startKetchup++;

			if (startKetchup > 180)
			{
				isKetchup = true;
				startKetchup = 0;
			}

			if (isKetchup)
			{
				ketchupTimer++;
				shoot->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x + 64)), PIXEL_TO_METERS((float32)(position.y + 84)) }, 0);
			}
			else
			{
				ketchupTimer = 0;
				shoot->body->SetTransform({ PIXEL_TO_METERS((float32)(-200)), PIXEL_TO_METERS((float32)(-200)) }, 0);
				
			}

			if (ketchupTimer >= 80)
			{
				jumpForce = -5.8f;
				isKetchup = false;
				startKetchup = 0;
				ketchupTimer = 0;
				
			}
		}
		else
		{
			shoot->body->SetTransform({ PIXEL_TO_METERS((float32)(-200)), PIXEL_TO_METERS((float32)(-200)) }, 0);
		}

		// Draw Debug Bounds area of effect
		bound.x = position.x - 75;
		bound.y = position.y - 60;
		bound.w = 300;
		bound.h = 350;

		if (app->physics->debug) app->render->DrawRectangle(bound, 0, 255, 0, 80);


		// Life of the Boss
		currentHp = &HP;
		currentHp->SetCurrentFrame(lifeCurrentframe);

		if (!canDmg)
		{
			dmgTimer++;
		}
		
		if (dmgTimer >= 25) {
			canDmg = true;
			dmgTimer = 0;
		}

		app->render->DrawTexture(lifeBoss, app->scene->player->position.x - 80, app->scene->player->position.y + 50, &currentHp->GetCurrentFrame());

		// Boss Death
		if (life <= 0)
		{
			
			if (isFacingLeft)
			{
				currentAnim = &Death;
				
			}
			else
			{
				currentAnim = &DeathRight;
			}

			pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y));
			deathTimer++;

			if (deathTimer >= 130)
			{
				isDead = true;
				app->scene->Disable();
				app->map->Disable();
				app->physics->Disable();
				app->entityManager->Disable();
				app->end->Enable();
			}
		}


		

		// Draw Boss
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 60);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 35);

		currentAnim->Update();
		
	}

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());


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
	case ColliderType::PLATFORM:
		if (startKetchup >= 100)
		{
			isKetchup = true;
		}
		break;
	case ColliderType::ATTACK:
		if (canDmg)
		{
			life--;
			lifeCurrentframe++;
			canDmg = false;
		}
		break;
	}
}