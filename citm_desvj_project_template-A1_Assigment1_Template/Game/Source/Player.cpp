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

	Idle.PushBack({ 0, 0, 30, 30 });
	Idle.PushBack({ 30, 0, 30, 30 });
	Idle.PushBack({ 60, 0, 30, 30 });
	Idle.PushBack({ 90, 0, 30, 30 });
	Idle.PushBack({ 120, 0, 30, 30 });
	Idle.PushBack({ 150, 0, 30, 30 });

	Idle.speed = 0.2f;
	Idle.loop = true;

	Run.PushBack({ 0, 60, 30, 30 });
	Run.PushBack({ 30, 60, 30, 30 });
	Run.PushBack({ 60, 60, 30, 30 });
	Run.PushBack({ 90, 60, 30, 30 });
	Run.PushBack({ 120, 60, 30, 30 });
	Run.PushBack({ 150, 60, 30, 30 });
	Run.PushBack({ 180, 60, 30, 30 });
	Run.PushBack({ 210, 60, 30, 30 });

	Run.speed = 0.2f;
	Run.loop = true;

	ChargeJump.PushBack({ 0, 30, 30, 30 });
	ChargeJump.PushBack({ 30, 30, 30, 30 });
	ChargeJump.PushBack({ 60, 30, 30, 30 });

	ChargeJump.speed = 0.1f;
	ChargeJump.loop = false;

	Jump.PushBack({ 90, 30, 30, 30 });
	Jump.PushBack({ 120, 30, 30, 30 });
	Jump.PushBack({ 150, 30, 30, 30 });
	Jump.PushBack({ 180, 30, 30, 30 });
	Jump.PushBack({ 210, 30, 30, 30 });
	Jump.PushBack({ 240, 30, 30, 30 });
	Jump.PushBack({ 270, 30, 30, 30 });
	Jump.PushBack({ 300, 30, 30, 30 });
	Jump.PushBack({ 330, 30, 30, 30 });
	Jump.PushBack({ 360, 30, 30, 30 });
	Jump.PushBack({ 390, 30, 30, 30 });
	Jump.PushBack({ 420, 30, 30, 30 });

	Jump.speed = 0.1f;
	Jump.loop = false;

}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	powerJump.h = 0;
	powerJump.w = 0;
	powerJump.x = position.x;
	powerJump.y = position.y - 30;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	app->audio->PlayMusic("Assets/Audio/Music/LeFestinLetHimCook.mp3", 0.0f);

	return true;
}

bool Player::Update(float dt)
{
	currentAnim = &Idle;
	b2Vec2 vel = b2Vec2(0, 0);
	if (godMode == false) {
		vel = b2Vec2(0, -GRAVITY_Y);
	}
	else {
		vel = b2Vec2(0, 0);
	}
	
	uint scale = app->win->GetScale();

	
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && player != jumpState::JUMPING) {
		currentAnim = &ChargeJump;
		if (power < 0.6f) {
			power += 0.025f;
			powerJump.w += 1;
		}
		
	} else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX = mouseX;
		mouseY = mouseY - app->render->camera.y;
		savePos = position.x * scale;
		savePosY = position.y * scale;

		triX = savePos - mouseX;
		triY = savePosY - mouseY;

		powerJump.w = 0;
		
		
		player = jumpState::POWER_JUMP;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		currentAnim = &Run;
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
		vel.x *= power*0.75;
		vel.x *= dt*0.75;
		vel.y *= power*0.75;
		vel.y *= dt*0.75;
		
		break;
	case Player::FLOOR:
		speedy = 1.0f;
		b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
		
		break;
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

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

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim->GetCurrentFrame(), 0.0f, 0, flip);

	app->render->DrawTexture(texture, position.x, position.y, &currentAnim ->GetCurrentFrame());
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
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}