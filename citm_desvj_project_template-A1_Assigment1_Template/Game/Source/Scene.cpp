#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "SceneMenu.h"

#include "SDL_mixer/include/SDL_mixer.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
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

	sceneParameter = config;

	if (app->map->isMap1) 
	{

		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();


		if (config.child("michelin")) {
			star = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
			star->parameters = config.child("michelin");
		}

		if (config.child("michelin2")) {
			star2 = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
			star2->parameters = config.child("michelin2");
		}

		if (config.child("michelin3")) {
			star3 = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
			star3->parameters = config.child("michelin3");
		}

		if (config.child("item")) {
			item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item->parameters = config.child("item");
		}

		if (config.child("item2")) {
			item2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item2->parameters = config.child("item2");
		}

		if (config.child("item3")) {
			item3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item3->parameters = config.child("item3");
		}

		if (config.child("item4")) {
			item4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item4->parameters = config.child("item4");
		}

		if (config.child("player")) {
			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			player->parameters = config.child("player");
		}

		if (config.child("enemy")) {
			enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
			enemy->parameters = config.child("enemy");
		}

		if (config.child("enemy2")) {
			enemy2 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
			enemy2->parameters = config.child("enemy2");
		}

		if (config.child("FlyEnemy")) {
			flyenemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
			flyenemy->parameters = config.child("FlyEnemy");
		}

		if (config.child("FlyEnemy2")) {
			flyenemy2 = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
			flyenemy2->parameters = config.child("FlyEnemy2");
		}

		if (config.child("Pozo")) {
			pozo = (Pozo*)app->entityManager->CreateEntity(EntityType::POZO);
			pozo->parameters = config.child("Pozo");
		}

		if (config.child("map")) {
			//Get the map name from the config file and assigns the value in the module
			app->map->name = config.child("map").attribute("name").as_string();
			app->map->path = config.child("map").attribute("path").as_string();
		}

		audioPath = config.child("musicFile").attribute("path").as_string();
	}
	else /*Level 2*/
	{
		app->map->name = config.child("map1").attribute("name").as_string();
		app->map->path = config.child("map1").attribute("path").as_string();

		if (config.child("player2")) {
			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			player->parameters = config.child("player2");
		}

		if (config.child("boss")) {
			boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
			boss->parameters = config.child("boss");
		}

		if (config.child("Pozo2")) {
			pozo2 = (Pozo*)app->entityManager->CreateEntity(EntityType::POZO);
			pozo2->parameters = config.child("Pozo2");
		}
	}

	
	
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	
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

	checkPoint = app->tex->Load("Assets/Textures/Checkpoint.png");

	checkPointUI = app->tex->Load("Assets/Textures/NumCheck5.png");

	SliderTex = app->tex->Load("Assets/Textures/SliderSprite.png");
	ButtonSlider = app->tex->Load("Assets/Textures/ButtonSlider.png");

	
	
	CreateOptionsButtons();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	firstStart = false;

	return true;
}

void Scene::CreateOptionsButtons()
{
	SDL_Rect SliderPos = { 305, player->position.y - 65, 100, 10 };
	SliderScene = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 9, "Slider", SliderPos, this, SliderTex, ButtonSlider);

	SDL_Rect VSyncPos = { 310, player->position.y - 105, 25, 25 };
	VSyncScene = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "VSync", VSyncPos, this, SliderTex);
	if (app->sceneMenu->isVSyncCheck)
	{
		VSyncScene->checked = true;
	}
	else {
		VSyncScene->checked = false;
	}

	SDL_Rect FullScreenPos = { 375, player->position.y - 105, 25, 25 };
	FullScreenScene = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 11, "FullScreen", FullScreenPos, this, SliderTex);
	if (app->sceneMenu->isFullScreenCheck)
	{
		FullScreenScene->checked = true;
	}
	else {
		FullScreenScene->checked = false;
	}
}

void Scene::DeleteButtons()
{
	SliderScene = nullptr;
	VSyncScene = nullptr;
	FullScreenScene = nullptr;
}

void Scene::StartMusic()
{
	app->audio->PlayMusic(audioPath.GetString(), 0.0f);
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 

	player->isScene = true;

	int scale = app->win->GetScale();

	if ((app->sceneMenu->isSettingsActive && app->scene->player->options) && app->sceneMenu->positionCartel > (-app->render->camera.x + app->render->camera.w - 472) / scale)
	{
		app->sceneMenu->positionCartel -= 2;
	}
	else if ((!app->sceneMenu->isSettingsActive && app->scene->player->options == false) && app->sceneMenu->positionCartel < (-app->render->camera.x + app->render->camera.w) / scale)
	{
		app->sceneMenu->positionCartel += 2;
	}

	if ((app->sceneMenu->isSettingsActive && app->scene->player->options) && app->sceneMenu->positionCartel <= (-app->render->camera.x + app->render->camera.w - 472) / scale)
	{
		app->sceneMenu->showOptions = true;
	}
	else {
		app->sceneMenu->showOptions = false;
	}

	app->render->DrawTexture(app->sceneMenu->Cartel, app->sceneMenu->positionCartel, player->position.y - 200);

	if(app->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		int stop = 0;
	}
		

	if (checkPoints == 0) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck5.png");
	} 
	else if (checkPoints == 1) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck4.png");
	}
	else if (checkPoints == 2) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck3.png");
	}
	else if (checkPoints == 3) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck2.png");
	}
	else if (checkPoints == 4) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck1.png");
	}
	else if (checkPoints == 5) {
		checkPointUI = app->tex->Load("Assets/Textures/NumCheck0.png");
	}

	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);

	if (checkPoints < 5) {
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveRequest();
			isSaved = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) 
		{
			isF6 = true;
			app->LoadRequest();
			
		}
			
	}
	
	if (checkPoints >= 1) app->render->DrawTexture(checkPoint, check1.x, check1.y);
	if (checkPoints >= 2) app->render->DrawTexture(checkPoint, check2.x, check2.y);
	if (checkPoints >= 3) app->render->DrawTexture(checkPoint, check3.x, check3.y);
	if (checkPoints >= 4) app->render->DrawTexture(checkPoint, check4.x, check4.y);
	if (checkPoints >= 5) app->render->DrawTexture(checkPoint, check5.x, check5.y);

	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		app->scene->Disable();
		app->sceneMenu->Enable();
		app->map->Disable();
		app->entityManager->Disable();
		isInScene = false;
		isF6 = false;
	}

	
	if (app->map->isMap1)
	{
		app->render->DrawTexture(checkPointUI, app->render->camera.x + 20, player->position.y - 185);
	}
	else if (!app->map->isMap1)
	{
		app->render->DrawTexture(checkPointUI, (-app->render->camera.x) / scale + 20, player->position.y - 185);
	}
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//volume sounds
	Mix_VolumeMusic(volume);
	Mix_Volume(-1, volume);

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

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}

bool Scene::LoadState(pugi::xml_node node)
{
	player->position.x = node.child("positionPlayer").attribute("x").as_int();
	player->position.y = node.child("positionPlayer").attribute("y").as_int();
	player->lifeFrame = node.child("positionPlayer").attribute("lifeLost").as_int();

	player->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionPlayer").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionPlayer").attribute("y").as_int()) }, 0);

	if (!node.child("positionEnemy").attribute("isDead").as_bool()) {
		
		enemy->pendingDelete = true;
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = sceneParameter.child("enemy");
		enemy->Awake();
		enemy->Start();
		enemy->position.x = node.child("positionEnemy").attribute("x").as_int();
		enemy->position.y = node.child("positionEnemy").attribute("y").as_int();
		enemy->isDead = node.child("positionEnemy").attribute("isDead").as_bool();
		enemy->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionEnemy").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionEnemy").attribute("y").as_int()) }, 0);
	}
	

	
	if (!node.child("positionEnemy2").attribute("isDead").as_bool()) {
		
		enemy2->pendingDelete = true;
		enemy2 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy2->parameters = sceneParameter.child("enemy2");
		enemy2->Awake();
		enemy2->Start();
		enemy2->position.x = node.child("positionEnemy2").attribute("x").as_int();
		enemy2->position.y = node.child("positionEnemy2").attribute("y").as_int();
		enemy2->isDead = node.child("positionEnemy2").attribute("isDead").as_bool();
		enemy2->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionEnemy2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionEnemy2").attribute("y").as_int()) }, 0);
	}
	

	
	if (!node.child("positionFlyEnemy").attribute("isDead").as_bool()) {
		
		flyenemy->pendingDelete = true;
		flyenemy = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
		flyenemy->parameters = sceneParameter.child("FlyEnemy");
		flyenemy->Awake();
		flyenemy->Start();
		flyenemy->position.x = node.child("positionFlyEnemy").attribute("x").as_int();
		flyenemy->position.y = node.child("positionFlyEnemy").attribute("y").as_int();
		flyenemy->isDead = node.child("positionFlyEnemy").attribute("isDead").as_bool();
		flyenemy->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionFlyEnemy").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionFlyEnemy").attribute("y").as_int()) }, 0);
		
	}
	
	
	if (!node.child("positionFlyEnemy2").attribute("isDead").as_bool()) {
		
		flyenemy2->pendingDelete = true;
		flyenemy2 = (FlyEnemy*)app->entityManager->CreateEntity(EntityType::FLY_ENEMY);
		flyenemy2->parameters = sceneParameter.child("FlyEnemy2");
		flyenemy2->Awake();
		flyenemy2->Start();
		flyenemy2->position.x = node.child("positionFlyEnemy2").attribute("x").as_int();
		flyenemy2->position.y = node.child("positionFlyEnemy2").attribute("y").as_int();
		flyenemy2->isDead = node.child("positionFlyEnemy2").attribute("isDead").as_bool();
		flyenemy2->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionFlyEnemy2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionFlyEnemy2").attribute("y").as_int()) }, 0);
	}
	
	if (app->map->isMap1 == false)
	{
		if (!node.child("positionBoss").attribute("isDead").as_bool()) {

			boss->pendingDelete = true;
			boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
			boss->parameters = sceneParameter.child("boss");
			boss->Awake();
			boss->Start();
			boss->position.x = node.child("positionBoss").attribute("x").as_int();
			boss->position.y = node.child("positionBoss").attribute("y").as_int();
			boss->isDead = node.child("positionBoss").attribute("isDead").as_bool();
			boss->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("positionBoss").attribute("x").as_int()), PIXEL_TO_METERS(node.child("positionBoss").attribute("y").as_int()) }, 0);
		}
	}
	
		star->pendingDelete = true;
		star = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
		star->parameters = sceneParameter.child("michelin");
		star->Awake();
		star->Start();
		star->position.x = node.child("michelin1").attribute("x").as_int();
		star->position.y = node.child("michelin1").attribute("y").as_int();
		star->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("michelin1").attribute("x").as_int()), PIXEL_TO_METERS(node.child("michelin1").attribute("y").as_int()) }, 0);
	
		star2->pendingDelete = true;
		star2 = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
		star2->parameters = sceneParameter.child("michelin2");
		star2->Awake();
		star2->Start();
		star2->position.x = node.child("michelin2").attribute("x").as_int();
		star2->position.y = node.child("michelin2").attribute("y").as_int();
		star2->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("michelin2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("michelin2").attribute("y").as_int()) }, 0);

		star3->pendingDelete = true;
		star3 = (Michelin*)app->entityManager->CreateEntity(EntityType::MICHELIN);
		star3->parameters = sceneParameter.child("michelin3");
		star3->Awake();
		star3->Start();
		star3->position.x = node.child("michelin3").attribute("x").as_int();
		star3->position.y = node.child("michelin3").attribute("y").as_int();
		star3->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("michelin3").attribute("x").as_int()), PIXEL_TO_METERS(node.child("michelin3").attribute("y").as_int()) }, 0);

		item->pendingDelete = true;
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = sceneParameter.child("item");
		item->Awake();
		item->Start();
		item->position.x = node.child("item1").attribute("x").as_int();
		item->position.y = node.child("item1").attribute("y").as_int();
		item->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("item1").attribute("x").as_int()), PIXEL_TO_METERS(node.child("item1").attribute("y").as_int()) }, 0);

		item2->pendingDelete = true;
		item2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item2->parameters = sceneParameter.child("item2");
		item2->Awake();
		item2->Start();
		item2->position.x = node.child("item2").attribute("x").as_int();
		item2->position.y = node.child("item2").attribute("y").as_int();
		item2->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("item2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("item2").attribute("y").as_int()) }, 0);

		item3->pendingDelete = true;
		item3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item3->parameters = sceneParameter.child("item3");
		item3->Awake();
		item3->Start();
		item3->position.x = node.child("item3").attribute("x").as_int();
		item3->position.y = node.child("item3").attribute("y").as_int();
		item3->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("item3").attribute("x").as_int()), PIXEL_TO_METERS(node.child("item3").attribute("y").as_int()) }, 0);

		item4->pendingDelete = true;
		item4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item4->parameters = sceneParameter.child("item4");
		item4->Awake();
		item4->Start();
		item4->position.x = node.child("item4").attribute("x").as_int();
		item4->position.y = node.child("item4").attribute("y").as_int();
		item4->pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("item4").attribute("x").as_int()), PIXEL_TO_METERS(node.child("item4").attribute("y").as_int()) }, 0);




	return true;
}

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node playerNode = node.append_child("positionPlayer");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("lifeLost").set_value(player->lifeFrame);
	

	if (checkPoints == 0) {
		check1 = player->position;
	}
	else if (checkPoints == 1) {
		check2 = player->position;
	}
	else if (checkPoints == 2) {
		check3 = player->position;
	}
	else if (checkPoints == 3) {
		check4 = player->position;
	}
	else if (checkPoints == 4) {
		check5 = player->position;
	}

	checkPoints++;

	pugi::xml_node enemyNode = node.append_child("positionEnemy");
	enemyNode.append_attribute("x").set_value(enemy->position.x);
	enemyNode.append_attribute("y").set_value(enemy->position.y);
	enemyNode.append_attribute("isDead").set_value(enemy->isDead);

	pugi::xml_node enemy2Node = node.append_child("positionEnemy2");
	enemy2Node.append_attribute("x").set_value(enemy2->position.x);
	enemy2Node.append_attribute("y").set_value(enemy2->position.y);
	enemy2Node.append_attribute("isDead").set_value(enemy2->isDead);

	pugi::xml_node flyEnemyNode = node.append_child("positionFlyEnemy");
	flyEnemyNode.append_attribute("x").set_value(flyenemy->position.x);
	flyEnemyNode.append_attribute("y").set_value(flyenemy->position.y);
	flyEnemyNode.append_attribute("isDead").set_value(flyenemy->isDead);

	pugi::xml_node flyEnemy2Node = node.append_child("positionFlyEnemy2");
	flyEnemy2Node.append_attribute("x").set_value(flyenemy2->position.x);
	flyEnemy2Node.append_attribute("y").set_value(flyenemy2->position.y);
	flyEnemy2Node.append_attribute("isDead").set_value(flyenemy2->isDead);

	if (app->map->isMap1 == false)
	{
		pugi::xml_node bossNode = node.append_child("positionBoss");
		bossNode.append_attribute("x").set_value(boss->position.x);
		bossNode.append_attribute("y").set_value(boss->position.y);
		bossNode.append_attribute("isDead").set_value(boss->isDead);
	}

	pugi::xml_node michelin1Node = node.append_child("michelin1");
	michelin1Node.append_attribute("x").set_value(star->position.x);
	michelin1Node.append_attribute("y").set_value(star->position.y);

	pugi::xml_node michelin2Node = node.append_child("michelin2");
	michelin2Node.append_attribute("x").set_value(star2->position.x);
	michelin2Node.append_attribute("y").set_value(star2->position.y);

	pugi::xml_node michelin3Node = node.append_child("michelin3");
	michelin3Node.append_attribute("x").set_value(star3->position.x);
	michelin3Node.append_attribute("y").set_value(star3->position.y);

	pugi::xml_node item1Node = node.append_child("item1");
	item1Node.append_attribute("x").set_value(item->position.x);
	item1Node.append_attribute("y").set_value(item->position.y);

	pugi::xml_node item2Node = node.append_child("item2");
	item2Node.append_attribute("x").set_value(item2->position.x);
	item2Node.append_attribute("y").set_value(item2->position.y);

	pugi::xml_node item3Node = node.append_child("item3");
	item3Node.append_attribute("x").set_value(item3->position.x);
	item3Node.append_attribute("y").set_value(item3->position.y);

	pugi::xml_node item4Node = node.append_child("item4");
	item4Node.append_attribute("x").set_value(item4->position.x);
	item4Node.append_attribute("y").set_value(item4->position.y);

	


	return true;
}

