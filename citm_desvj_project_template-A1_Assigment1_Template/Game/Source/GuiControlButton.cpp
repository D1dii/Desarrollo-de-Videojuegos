#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->texture = tex;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	int scale = app->win->GetScale();
	//SetTexture(texture);

	if (state != GuiControlState::DISABLED)
	{
		
		app->input->GetMousePosition(mouseX, mouseY);
		int mouseYBounds = mouseY - app->render->camera.y / scale;

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x / scale && mouseX < bounds.x / scale + bounds.w / scale && mouseYBounds > bounds.y && mouseYBounds < bounds.y + bounds.h) {

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
			break;
		}

		
	}

	return false;
}