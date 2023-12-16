#include "Player.h"
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
}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	texturePath = parameters.attribute("texturepath").as_string();

	InitAnims();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
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

	SString audioPath = parameters.child("musicFile").attribute("path").as_string();

	app->audio->PlayMusic(audioPath.GetString(), 0.0f);
	

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
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && player != jumpState::POWER_JUMP) {
		if (isFacingLeft != true) {
			currentAnim = &ChargeJump;
		}
		else {
			currentAnim = &ChargeJumpLeft;
		}
		if (power < 0.6f) {
			power += 0.025f;
			powerJump.w += 1;
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) isFacingLeft = false;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) isFacingLeft = true;
		}
		
	} else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && player != jumpState::POWER_JUMP) {
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX = mouseX;
		mouseY = mouseY - app->render->camera.y;
		savePos = position.x * scale;
		savePosY = position.y * scale;

		/*triX = savePos - mouseX;
		triY = savePosY - mouseY;*/

		/*if (mouseX < savePos) {
			isFacingLeft = true;
		}
		else {
			isFacingLeft = false;
		}*/

		if (isFacingLeft == true) {
			triX = 250;
			triY = 400;
		}
		else {
			triX = -250;
			triY = 400;
		}

		powerJump.w = 0;
		
		
		player = jumpState::POWER_JUMP;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		currentAnim = &RunLeft;
		isFacingLeft = true;
		if (godMode == false) {
			vel = b2Vec2(-speedx * dt, -GRAVITY_Y);
		}
		else {
			vel = b2Vec2(-speedx * dt, 0);
		}
			
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		isFacingLeft = false;
		currentAnim = &Run;
		if (godMode == false) {
			vel = b2Vec2(speedx * dt, -GRAVITY_Y);
		}
		else {
			vel = b2Vec2(speedx * dt, 0);
		}
		
		
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && godMode == true) {
		currentAnim = &Run;
		vel = b2Vec2(0, -speedx * dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && godMode == true) {
		currentAnim = &Run;
		vel = b2Vec2(0, speedx * dt);

	}

	
	switch (player)
	{
	case Player::JUMPING:
		
		vel = b2Vec2(0, -speedy * dt * 2);
		speedy -= jumpa;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
			isFacingLeft = true;
			vel = b2Vec2(-speedx * dt, -speedy * dt * 2);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
			isFacingLeft = false;
			vel = b2Vec2(speedx * dt, -speedy * dt * 2);
		}
		
		break;
	case Player::POWER_JUMP:
		angle = atan2(triY, triX) * DEGTORAD;
		jumpX = speedPower * cos(angle);
		jumpY = -speedPower * sin(angle);

		vel = b2Vec2(-triX, -triY);
		triY += GRAVITY_Y;
		vel.Normalize();
		vel.x *= power*0.55;
		vel.x *= dt*0.55;
		vel.y *= power*0.55;
		vel.y *= dt*0.55;
		
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

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - 5);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - 7);

	// Debug options

	// GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	// Respawn on the start
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		position.x = 100;
		position.y = 2150;
		pbody->body->SetTransform({PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y)}, 0);
	}

	// Spawn on debug checkpoints
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		position.x = 368;
		position.y = 1568;
		pbody->body->SetTransform({PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = 80;
		position.y = 912;
		pbody->body->SetTransform({ PIXEL_TO_METERS((float32)position.x), PIXEL_TO_METERS((float32)position.y) }, 0);
	}

	// Show Power Jump bar
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
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

	currentAnim->Update();



	SDL_RendererFlip flip = (isFacingLeft) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame());
	
	app->render->DrawRectangle(powerJump, 255, 0, 0, 255);

	app->render->camera.y = (-position.y + 230) * scale;

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

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
			justFall = true;
			fallTimer = 0;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}