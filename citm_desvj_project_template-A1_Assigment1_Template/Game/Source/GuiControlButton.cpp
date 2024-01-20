#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	uint scale = app->win->GetScale();

	if (state != GuiControlState::DISABLED)
	{
		
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x / scale && mouseX < bounds.x / scale + bounds.w / scale && mouseY > bounds.y / scale && mouseY < bounds.y / scale + bounds.h / scale) {

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

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			NewGameTest = app->tex->Load("Assets/Textures/NewGameDisabled.png");
			app->render->DrawTexture(NewGameTest, bounds.x / scale, 2100);
			break;
		case GuiControlState::NORMAL:
			NewGameTest = app->tex->Load("Assets/Textures/NewGameNormal.png");
			app->render->DrawTexture(NewGameTest, bounds.x / scale, 2100);
			break;
		case GuiControlState::FOCUSED:
			NewGameTest = app->tex->Load("Assets/Textures/NewGameFocused.png");
			app->render->DrawTexture(NewGameTest, bounds.x / scale, 2100);
			break;
		case GuiControlState::PRESSED:
			NewGameTest = app->tex->Load("Assets/Textures/NewGamePressed.png");
			app->render->DrawTexture(NewGameTest, bounds.x / scale, 2100);
			break;
		}

		
	}

	return false;
}