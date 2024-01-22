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
			}
			else if (buttonID == 5)
			{
				app->sceneMenu->ret = false;
			}
			break;
		}

		
	}

	return false;
}