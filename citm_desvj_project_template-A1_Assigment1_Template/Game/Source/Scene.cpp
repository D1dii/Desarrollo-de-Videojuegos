#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("enemy")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy");
	}

	if (config.child("FlyEnemy")) {
		flyenemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
		flyenemy->parameters = config.child("FlyEnemy");
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tilewidth,
		app->map->mapData.tileheight,
		app->map->mapData.tilesets.Count());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 

	if(app->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		int stop = 0;
	}
		

	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

iPoint Scene::GetPLayerPosition() {
	return player->position;
}

bool Scene::LoadState(pugi::xml_node node)
{
	player->position.x = node.child("positionPlayer").attribute("x").as_int();
	player->position.y = node.child("positionPlayer").attribute("y").as_int();

	player->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionPlayer").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionPlayer").attribute("y").as_int()) }, 0);


	enemy->position.x = node.child("positionEnemy").attribute("x").as_int();
	enemy->position.y = node.child("positionEnemy").attribute("y").as_int();

	enemy->enemyCollider->body->SetTransform({ PIXEL_TO_METERS(node.child("positionEnemy").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionEnemy").attribute("y").as_int()) }, 0);

	flyenemy->position.x = node.child("positionFlyEnemy").attribute("x").as_int();
	flyenemy->position.y = node.child("positionFlyEnemy").attribute("y").as_int();

	flyenemy->enemyCollider->body->SetTransform({ PIXEL_TO_METERS(node.child("positionFlyEnemy").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionFlyEnemy").attribute("y").as_int()) }, 0);

	return true;
}

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node playerNode = node.append_child("positionPlayer");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);

	pugi::xml_node enemyNode = node.append_child("positionEnemy");
	enemyNode.append_attribute("x").set_value(enemy->position.x);
	enemyNode.append_attribute("y").set_value(enemy->position.y);

	pugi::xml_node flyEnemyNode = node.append_child("positionFlyEnemy");
	flyEnemyNode.append_attribute("x").set_value(flyenemy->position.x);
	flyEnemyNode.append_attribute("y").set_value(flyenemy->position.y);

	return true;
}