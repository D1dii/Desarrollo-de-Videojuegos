#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex = NULL);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	SDL_Rect NormalBounds = { 0, 0, 128, 32 };
	SDL_Rect DisabledBounds = { 0, 32, 128, 32 };
	SDL_Rect FocusedBounds = { 0, 64, 128, 32 };
	SDL_Rect PressedBounds = { 0, 96, 128, 32 };
};

#endif // __GUICONTROLBUTTON_H__
