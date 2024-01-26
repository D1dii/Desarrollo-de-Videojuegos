#ifndef __SCREENEND_H__
#define __SCREENEND_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

#include "Physics.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

struct SDL_Texture;

class ScreenEnd : public Module
{
public:

	ScreenEnd(bool startEnabled);

	// Destructor
	virtual ~ScreenEnd();

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

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);



private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

public:

	uint victoryFx;

	pugi::xml_node sceneParameter;
	bool ret = true;

	SString audioPath;

	

};

#endif // __SCREENEND_H__

