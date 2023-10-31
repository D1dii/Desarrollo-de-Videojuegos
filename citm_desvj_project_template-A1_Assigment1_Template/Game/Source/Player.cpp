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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
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

	
	powerJump.h = 20;
	powerJump.w = 0;
	powerJump.x = position.x;
	powerJump.y = position.y - 30;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && player != jumpState::JUMPING) {
		if (power < 2.5f) {
			power += 0.025f;
			powerJump.w += 1;
		}
		
	} else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		SDL_GetMouseState(&mouseX, &mouseY);
		savePos = position.x;
		savePosY = position.y;

		triX = savePos - mouseX;
		triY = savePosY - mouseY;

		powerJump.w = 0;
		
		
		player = jumpState::POWER_JUMP;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		vel = b2Vec2(-speedx*dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		vel = b2Vec2(speedx*dt, -GRAVITY_Y);
	}

	switch (player)
	{
	case Player::JUMPING:
		
		vel = b2Vec2(0, -speedy * dt * 2);
		speedy -= jumpa;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
			vel = b2Vec2(-speedx * dt, -speedy * dt * 2);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
			vel = b2Vec2(speedx * dt, -speedy * dt * 2);
		}
		break;
	case Player::POWER_JUMP:
		

		angle = atan2(triY, triX) * DEGTORAD;
		jumpX = speedPower * cos(angle);
		jumpY = -speedPower * sin(angle);

		vel = b2Vec2(-triX, -triY);
		triY += GRAVITY_Y * 2;
		vel.Normalize();
		vel.x *= power;
		vel.x *= dt;
		vel.y *= power;
		vel.y *= dt;
		
		
		
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

	powerJump.x = position.x;
	powerJump.y = position.y - 30;

	app->render->DrawTexture(texture, position.x, position.y);
	app->render->DrawRectangle(powerJump, 255, 0, 0, 255);

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