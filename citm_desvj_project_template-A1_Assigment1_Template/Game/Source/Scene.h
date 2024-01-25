#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "Pozo.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Michelin.h"
#include "Boss.h"

#include "Physics.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return player position
	iPoint GetPLayerPosition();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

	void StartMusic();

	void CreateOptionsButtons();

	void DeleteButtons();
	

private:
	SDL_Texture* img;
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	
	GuiControlButton* gcButtom;
	GuiControlButton* test;
public:

	pugi::xml_node sceneParameter;
	

	SString audioPath;

	SDL_Texture* checkPoint = nullptr;
	SDL_Texture* checkPointUI = nullptr;

	Player* player;
	Enemy* enemy;
	Enemy* enemy2;
	FlyEnemy* flyenemy;
	FlyEnemy* flyenemy2;
	Pozo* pozo;
	Pozo* pozo2;
	Item* item;
	Item* item2;
	Item* item3;
	Item* item4;
	Michelin* star;
	Michelin* star2;
	Michelin* star3;
	Boss* boss;
	
	

	int checkPoints = 0;

	iPoint check1;
	iPoint check2;
	iPoint check3;
	iPoint check4;
	iPoint check5;

	SDL_Texture* SliderTex = nullptr;
	SDL_Texture* ButtonSlider = nullptr;

	GuiSlider* SliderScene;
	GuiCheckBox* VSyncScene;
	GuiCheckBox* FullScreenScene;

	bool firstStart = true;
	bool isSaved = false;
	bool isInScene = false;
	bool isF6 = false;
	

	float volume = 90;

};

#endif // __SCENE_H__