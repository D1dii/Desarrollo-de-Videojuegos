#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "GuiManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{

	name.Create("Player");
	isFacingLeft = false;
}

Player::~Player() {

}

void Player::InitAnims() {
	// Idle
	for (pugi::xml_node node = parameters.child("Idle").child("pushback"); node; node = node.next_sibling("pushback")) {
		Idle.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Idle.speed = parameters.child("Idle").attribute("animspeed").as_float();
	Idle.loop = parameters.child("Idle").attribute("loop").as_bool();

	// IdleLeft
	for (pugi::xml_node node = parameters.child("IdleLeft").child("pushback"); node; node = node.next_sibling("pushback")) {
		IdleLeft.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	IdleLeft.speed = parameters.child("IdleLeft").attribute("animspeed").as_float();
	IdleLeft.loop = parameters.child("IdleLeft").attribute("loop").as_bool();

	// Run
	for (pugi::xml_node node = parameters.child("Run").child("pushback"); node; node = node.next_sibling("pushback")) {
		Run.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Run.speed = parameters.child("Run").attribute("animspeed").as_float();
	Run.loop = parameters.child("Run").attribute("loop").as_bool();

	// RunLeft
	for (pugi::xml_node node = parameters.child("RunLeft").child("pushback"); node; node = node.next_sibling("pushback")) {
		RunLeft.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	RunLeft.speed = parameters.child("RunLeft").attribute("animspeed").as_float();
	RunLeft.loop = parameters.child("RunLeft").attribute("loop").as_bool();

	// ChargeJump
	for (pugi::xml_node node = parameters.child("ChargeJump").child("pushback"); node; node = node.next_sibling("pushback")) {
		ChargeJump.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	ChargeJump.speed = parameters.child("ChargeJump").attribute("animspeed").as_float();
	ChargeJump.loop = parameters.child("ChargeJump").attribute("loop").as_bool();

	// ChargeJumpLeft
	for (pugi::xml_node node = parameters.child("ChargeJumpLeft").child("pushback"); node; node = node.next_sibling("pushback")) {
		ChargeJumpLeft.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	ChargeJumpLeft.speed = parameters.child("ChargeJumpLeft").attribute("animspeed").as_float();
	ChargeJumpLeft.loop = parameters.child("ChargeJumpLeft").attribute("loop").as_bool();

	// Jump
	for (pugi::xml_node node = parameters.child("Jump").child("pushback"); node; node = node.next_sibling("pushback")) {
		Jump.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Jump.speed = parameters.child("Jump").attribute("animspeed").as_float();
	Jump.loop = parameters.child("Jump").attribute("loop").as_bool();

	// Fall
	for (pugi::xml_node node = parameters.child("Fall").child("pushback"); node; node = node.next_sibling("pushback")) {
		Fall.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	Fall.speed = parameters.child("Fall").attribute("animspeed").as_float();
	Fall.loop = parameters.child("Fall").attribute("loop").as_bool();

	// FallLeft
	for (pugi::xml_node node = parameters.child("FallLeft").child("pushback"); node; node = node.next_sibling("pushback")) {
		FallLeft.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	FallLeft.speed = parameters.child("FallLeft").attribute("animspeed").as_float();
	FallLeft.loop = parameters.child("FallLeft").attribute("loop").as_bool();

	// attack
	for (pugi::xml_node node = parameters.child("attack").child("pushback"); node; node = node.next_sibling("pushback")) {
		attack.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	attack.speed = parameters.child("attack").attribute("animspeed").as_float();
	attack.loop = parameters.child("attack").attribute("loop").as_bool();

	// attackLeft
	for (pugi::xml_node node = parameters.child("attackLeft").child("pushback"); node; node = node.next_sibling("pushback")) {
		attackLeft.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	attackLeft.speed = parameters.child("attackLeft").attribute("animspeed").as_float();
	attackLeft.loop = parameters.child("attackLeft").attribute("loop").as_bool();

	// lifes
	for (pugi::xml_node node = parameters.child("lifeAnim").child("pushback"); node; node = node.next_sibling("pushback")) {
		lifeAnim.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	lifeAnim.speed = parameters.child("lifeAnim").attribute("animspeed").as_float();
	lifeAnim.loop = parameters.child("lifeAnim").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("ChargeBar").child("pushback"); node; node = node.next_sibling("pushback")) {
		ChargeBar.PushBack({ node.attribute("x").as_int(),
						node.attribute("y").as_int(),
						node.attribute("width").as_int(),
						node.attribute("height").as_int() });
	}
	ChargeBar.speed = parameters.child("ChargeBar").attribute("animspeed").as_float();
	ChargeBar.loop = parameters.child("ChargeBar").attribute("loop").as_bool();
}

bool Player::Awake() {

	

	return true;
}

bool Player::Start() {

	if (app->scene->isSaved) {
		position.x = -300;
		position.y = 2500;
	}
	else if (app->scene->isSaved == false) {
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
	}
	

	texturePath = parameters.attribute("texturepath").as_string();


	InitAnims();

	//initilize textures
	texture = app->tex->Load(texturePath);
	hearts = app->tex->Load("Assets/Textures/hp.png");
	chargebar = app->tex->Load("Assets/Textures/charge bar.png");
	powerJump.h = 0;
	powerJump.w = 0;
	powerJump.x = position.x;
	powerJump.y = position.y - 30;

	int player[8] = {
		0, 0,
		12, 0,
		12, 16,
		0, 16,
	};

	
	pbody = app->physics->CreateChain(position.x, position.y, player, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	sword = app->physics->CreateRectangleSensor(position.x + 14, position.y, 12, 16, bodyType::DYNAMIC);
	sword->listener = this;
	sword->ctype = ColliderType::ATTACK;
	sword->body->SetGravityScale(0);
	
	
	jumpFx = app->audio->LoadFx(parameters.child("jumpAudio").attribute("path").as_string());
	
	
	return true;
}

bool Player::Update(float dt)
{
	if (justFall == true)
	{
		fallTimer++;
		if (isFacingLeft == false) {
			currentAnim = &Fall;
		}
		else {
			currentAnim = &FallLeft;
		}
	}
	else if (justFall == false)
	{
		if (isFacingLeft != true) {
			currentAnim = &Idle;
		}
		else {
			currentAnim = &IdleLeft;
		}
	}
	
	if (fallTimer >= 50) {
		justFall = false;
	}
	
	b2Vec2 vel = b2Vec2(0, 0);
	if (godMode == false) {
		vel = b2Vec2(0, -GRAVITY_Y);
	}
	else {
		vel = b2Vec2(0, 0);
	}
	
	uint scale = app->win->GetScale();

	
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && player != jumpState::POWER_JUMP && options == false) {
		isCharging = true;
		if (isFacingLeft != true) {
			currentAnim = &ChargeJump;
			chargebarcurrentanim = &ChargeBar;
		}
		else {
			currentAnim = &ChargeJumpLeft;
			chargebarcurrentanim = &ChargeBar;
		}
		if (power <= 0.6f) {
			power += 0.0125f;
			if (power < 0.2f) {
				ChargeBar.SetCurrentFrame(0);
			}
			else if (power >= 0.2f && power < 0.4f) {
				ChargeBar.SetCurrentFrame(1);
			}
			else  if (power >= 0.4f && power < 0.6f) {
				ChargeBar.SetCurrentFrame(2);
			}
			else  if (power >= 0.6f) {
				ChargeBar.SetCurrentFrame(3);
			}
			powerJump.w += 1;
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) isFacingLeft = false;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) isFacingLeft = true;
		}
		
	} else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && player != jumpState::POWER_JUMP && options == false) {
		isCharging = false;
		if (power < 0.2f) {
			power = 0.1;
		} else if (power >= 0.2f && power < 0.4f) {
			power = 0.2f;
		} else  if (power >= 0.4f && power < 0.6f) {
			power = 0.4f;
		} else  if (power >= 0.6f) {
			power = 0.6f;
		}
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX = mouseX;
		mouseY = mouseY - app->render->camera.y;
		savePos = position.x * scale;
		savePosY = position.y * scale;

		if (isFacingLeft == true) {
			triX = 250;
			triY = 400;
		}
		else {
			triX = -250;
			triY = 400;
		}

		powerJump.w = 0;
		
		app->audio->PlayFx(jumpFx);

		player = jumpState::POWER_JUMP;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && options == false) {
		currentAnim = &RunLeft;
		isFacingLeft = true;
		if (godMode == false) {
			vel = b2Vec2(-speedx * dt, -GRAVITY_Y);
		}
		else {
			vel = b2Vec2(-speedx * dt, 0);
		}
			
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && options == false) {
		isFacingLeft = false;
		currentAnim = &Run;
		if (godMode == false) {
			vel = b2Vec2(speedx * dt, -GRAVITY_Y);
		}
		else {
			vel = b2Vec2(speedx * dt, 0);
		}
		
		
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && godMode == true && options == false) {
		currentAnim = &Run;
		vel = b2Vec2(0, -speedx * dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && godMode == true && options == false) {
		currentAnim = &Run;
		vel = b2Vec2(0, speedx * dt);

	}

	
	switch (player)
	{
	case Player::JUMPING:
		
		vel = b2Vec2(0, -speedy * dt * 2);
		speedy -= jumpa;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && options == false) {
			isFacingLeft = true;
			vel = b2Vec2(-speedx * dt, -speedy * dt * 2);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && options == false) {
			isFacingLeft = false;
			vel = b2Vec2(speedx * dt, -speedy * dt * 2);
		}

		if (options == true) {
			app->physics->world->SetGravity({ 0,0 });
			
		}
		else if (options == false)
		{
			app->physics->world->SetGravity({ 0,10 });

		}
		
		break;
	case Player::POWER_JUMP:

		angle = atan2(triY, triX) * DEGTORAD;
		jumpX = speedPower * cos(angle);
		jumpY = -speedPower * sin(angle);
		vel = b2Vec2(-triX, -triY);
		triY += GRAVITY_Y;
		vel.Normalize();
		//deja esto por si tenemos que cambiar algo del salto
		//vel.x *= dt*0.55;
		//vel.y *= power;

		if (power == 0.1f) {
			vel.x *= power * 8;
			vel.y *= dt * 0.1;
		}
		else if (power == 0.2f) {
			vel.x *= power * 7;
			vel.y *= dt * 0.15;
		}
		else  if (power == 0.4f) {
			vel.x *= power * 6;
			vel.y *= dt * 0.2;
		}
		else  if (power == 0.6f) {
			vel.x *= power * 5;
			vel.y *= dt * 0.2;
		}
		
		break;
	case Player::FLOOR:
		speedy = 1.0f;
		b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
		if (justFall == true) {
			if (isFacingLeft == false) {
				currentAnim = &Fall;
			}
			else {
				currentAnim = &FallLeft;
			}
		}
		
		break;
	}

	// Attacking

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && options == false) {
		isAttacking = true;
	}

	if (isAttacking && options == false) {
		attTimer++;
		if (isFacingLeft == false) {
			sword->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x + 24)), PIXEL_TO_METERS((float32)(position.y + 15)) }, 0);
			currentAnim = &attack;
		}
		else if (isFacingLeft == true) {
			sword->body->SetTransform({ PIXEL_TO_METERS((float32)(position.x)), PIXEL_TO_METERS((float32)(position.y + 15)) }, 0);
			currentAnim = &attackLeft;
		}
		
		if (attTimer >= 70) {
			attTimer = 0;
			isAttacking = false;
		}
	}
	else {
		sword->body->SetTransform({ PIXEL_TO_METERS((float32)(0)), PIXEL_TO_METERS((float32)(0)) }, 0);
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 5);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 7);

	

	// Debug options

	// GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && options == false) {
		godMode = !godMode;
	}

	// Respawn on the start
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && options == false) {
		position.x = 90;
		position.y = 2172;
		pbody->body->SetTransform({PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y)}, 0);
	}

	// Spawn on debug checkpoints
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && options == false) {
		position.x = 416;
		position.y = 1440;
		pbody->body->SetTransform({PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && options == false) {
		position.x = 32;
		position.y = 1072;
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}

	// Show Power Jump bar
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && options == false) {
		showBar = !showBar;
		
		
	}


	if (showBar == false) {
		powerJump.h = 0;
	}
	else {
		powerJump.h = 20;
	}
	powerJump.x = position.x;
	powerJump.y = position.y - 30;

	lifeCurrentAnim = &lifeAnim;
	currentAnim->Update();
	lifeCurrentAnim->SetCurrentFrame(lifeFrame);

	if (!canDmg) {
		dmgTimer++;
	}

	if (dmgTimer >= 25) {
		canDmg = true;
		dmgTimer = 0;
	}

	if (lifes == 0) {
		app->LoadRequest();
		lifeCurrentAnim->SetCurrentFrame(0);
		lifeFrame = 0;
		lifes = 3;
	}

	SDL_RendererFlip flip = (isFacingLeft) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	if (isCharging == true) {
		app->render->DrawTexture(chargebar, position.x, position.y - 5, &chargebarcurrentanim->GetCurrentFrame());
	}

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	app->render->DrawTexture(hearts, app->render->camera.x + 20, position.y - 220, &lifeCurrentAnim->GetCurrentFrame());
	
	app->render->DrawRectangle(powerJump, 255, 0, 0, 255);

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		options = !options;
		app->sceneMenu->isSettingsActive = !app->sceneMenu->isSettingsActive;
		if (options) {
			app->guiManager->MoveButtons(305, position.y - 65, 9);
			app->guiManager->MoveButtons(310, position.y - 105, 10);
			app->guiManager->MoveButtons(375, position.y - 105, 11);
		}
		else if (!options)
		{
			app->scene->DeleteButtons();
		}
		
	}

	if (isScene) {
		app->render->camera.y = (-position.y + 230) * scale;
	}
	

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (options == false)
	{
		if (physA->ctype == ColliderType::PLAYER) {
			switch (physB->ctype)
			{
			case ColliderType::ITEM:
				LOG("Collision ITEM");
				app->audio->PlayFx(pickCoinFxId);
				break;
			case ColliderType::PLATFORM:
				LOG("Collision PLATFORM");
				if (player == jumpState::JUMPING || player == jumpState::POWER_JUMP) {
					player = jumpState::FLOOR;
					power = 0;
					charge = 0;
					justFall = true;
					fallTimer = 0;
				}
				break;
			case ColliderType::ENEMY_ATTACK:
				LOG("Collision ENEMY_ATTACK");

				if (canDmg && app->scene->isSaved == false) {
					lifes--;
					canDmg = false;
					if (lifeFrame < 4) {
						lifeFrame++;
					}
					else if (lifeFrame >= 4) {
						lifeFrame = 0;
					}
					lifeCurrentAnim->SetCurrentFrame(lifeFrame);
				}
				break;
			case ColliderType::ENEMY_ATTACK_SAVED:
				if (canDmg && app->scene->isSaved == true) {
					lifes--;
					canDmg = false;
					if (lifeFrame < 4) {
						lifeFrame++;
					}
					else if (lifeFrame >= 4) {
						lifeFrame = 0;
					}
					lifeCurrentAnim->SetCurrentFrame(lifeFrame);
				}
				break;
			case ColliderType::POZO:
				charge = 0;
				lifes = 0;
			case ColliderType::UNKNOWN:
				LOG("Collision UNKNOWN");
				break;

			}
		}
	}
	
	
}