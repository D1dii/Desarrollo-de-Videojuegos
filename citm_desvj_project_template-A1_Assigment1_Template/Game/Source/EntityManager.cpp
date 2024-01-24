#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "Pozo.h"
#include "Michelin.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item2;
	Entity* pEntity2 = NULL;

	for (item2 = entities.start; item2 != NULL && ret == true; item2 = item2->next)
	{
		pEntity2 = item2->data;

		if (pEntity2->active == false) continue;
		ret = item2->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::ENEMY:
		entity = new Enemy();
		break;
	case EntityType::FLY_ENEMY:
		entity = new FlyEnemy();
		break;
	case EntityType::POZO:
		entity = new Pozo();
		break;
	case EntityType::MICHELIN:
		entity = new Michelin();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->pendingDelete) {
			
			if(pEntity->pbody != nullptr) app->physics->DestroyObject(pEntity->pbody);
			if (pEntity->shoot != nullptr) app->physics->DestroyObject(pEntity->shoot);
			if (pEntity->explosion != nullptr) app->physics->DestroyObject(pEntity->explosion);
			if (pEntity->detect != nullptr) app->physics->DestroyObject(pEntity->detect);
			pEntity->pendingDelete = false;
			DestroyEntity(pEntity);

		}

		if (pEntity->active == false) continue;

		if(app->scene->player->options == false || item->data == app->scene->player)
		ret = item->data->Update(dt);
	}

	return ret;
}