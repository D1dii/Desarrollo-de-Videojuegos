#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "Player.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->texture = tex;
	this->buttonID = id;


	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	int scale = app->win->GetScale();
	
	SDL_Rect NormalBounds = { 0, 0, bounds.w, bounds.h };
	SDL_Rect DisabledBounds = { 0, bounds.h, bounds.w, bounds.h };
	SDL_Rect FocusedBounds = { 0, bounds.h * 2, bounds.w, bounds.h };
	SDL_Rect PressedBounds = { 0, bounds.h * 3, bounds.w, bounds.h };

	if (buttonID == 2 && app->scene->isSaved == false) 
	{
		state = GuiControlState::DISABLED;
	}
	else {
		state = GuiControlState::NORMAL;
	}

	if (state != GuiControlState::DISABLED)
	{
		
		app->input->GetMousePosition(mouseX, mouseY);
		int mouseYBounds = mouseY - app->render->camera.y / scale;

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x / scale && mouseX < bounds.x / scale + bounds.w && mouseYBounds > bounds.y && mouseYBounds < bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		
	}

	switch (state)
	{
	case GuiControlState::DISABLED:
		app->render->DrawTexture(texture, bounds.x / scale, bounds.y, &DisabledBounds);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawTexture(texture, bounds.x / scale, bounds.y, &NormalBounds);
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawTexture(texture, bounds.x / scale, bounds.y, &FocusedBounds);
		break;
	case GuiControlState::PRESSED:
		app->render->DrawTexture(texture, bounds.x / scale, bounds.y, &PressedBounds);

		if (buttonID == 1)
		{
			app->scene->Enable();
			app->sceneMenu->Disable();
			app->physics->Enable();
			app->map->Enable();
			app->entityManager->Enable();
			if (app->scene->firstStart == false)
			{
				app->AwakeScene();
				app->scene->isSaved = false;
				app->scene->checkPoints = 0;
				app->scene->player->Start();
				app->scene->enemy->Start();
				app->scene->enemy2->Start();
				app->scene->flyenemy->Start();
				app->scene->flyenemy2->Start();
				app->scene->pozo->Start();
			}
			app->scene->isInScene = true;
			app->scene->StartMusic();
		}
		else if (buttonID == 2 && app->scene->isSaved)
		{
			app->scene->Enable();
			app->sceneMenu->Disable();
			app->physics->Enable();
			app->map->Enable();
			app->entityManager->Enable();
			if (app->scene->firstStart == false)
			{
				app->AwakeScene();
				app->scene->player->Start();
				

				pugi::xml_document saveFile;
				pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

				if (saveFile.child("game_state").child("scene").child("positionEnemy").attribute("isDead").as_bool() == false)
				{
					app->scene->enemy->Start();
				}
				else {
					app->scene->enemy->isDead = true;
				}
				if (saveFile.child("game_state").child("scene").child("positionEnemy2").attribute("isDead").as_bool() == false)
				{
					app->scene->enemy2->Start();
				}
				else {
					app->scene->enemy2->isDead = true;
				}
				if (saveFile.child("game_state").child("scene").child("positionFlyEnemy").attribute("isDead").as_bool() == false)
				{
					app->scene->flyenemy->Start();
				}
				else {
					app->scene->flyenemy->isDead = true;
				}
				if (saveFile.child("game_state").child("scene").child("positionFlyEnemy2").attribute("isDead").as_bool() == false)
				{
					app->scene->flyenemy2->Start();
				}
				else {
					app->scene->flyenemy2->isDead = true;
				}
				app->scene->pozo->Start();
				app->LoadRequest();
				app->scene->player->lifes = 0;
			}
			app->scene->isInScene = true;
			app->scene->StartMusic();
		}
		else if (buttonID == 3)
		{
			app->sceneMenu->isSettingsActive = !app->sceneMenu->isSettingsActive;
		}
		else if (buttonID == 4)
		{
			app->sceneMenu->isCreditsActive = !app->sceneMenu->isCreditsActive;
		}
		else if (buttonID == 5)
		{
			app->sceneMenu->ret = false;
		}
		break;
	}

	return false;
}