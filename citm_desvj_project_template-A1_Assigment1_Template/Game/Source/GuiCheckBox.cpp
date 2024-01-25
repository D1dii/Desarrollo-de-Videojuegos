#include "GuiCheckBox.h"
#include "../App.h"
#include "../Scene.h"
#include "../GuiManager.h"
#include "../Audio.h"
#include "../Window.h"
#include "SceneMenu.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* tex) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = tex;
    this->id = id;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        int scale = app->win->GetScale();

        mouseX = mouseX / scale;
        int mouseYBounds = mouseY / scale - app->render->camera.y / scale;

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseYBounds > bounds.y) && (mouseYBounds < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                state = GuiControlState::SELECTED;
            }
        }
        else state = GuiControlState::NORMAL;
    }
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;

    case GuiControlState::NORMAL:

        if (id == 7 || id == 10)
        {
            if (checked) {
                app->render->DrawRectangle(bounds, 0, 255, 0, 128, true, true);
                app->isVSync = true;
                app->sceneMenu->isVSyncCheck = true;
            }


            if (!checked) {
                app->render->DrawRectangle(bounds, 0, 255, 0, 128, false, true);
                app->isVSync = false;
                app->sceneMenu->isVSyncCheck = false;
            }
        }

        if (id == 8 || id == 11)
        {
            if (checked) {
                app->render->DrawRectangle(bounds, 0, 255, 0, 128, true, true);
                
            }


            if (!checked) {
                app->render->DrawRectangle(bounds, 0, 255, 0, 128, false, true);
                
            }
        }
        
        
            
      
        break;

    case GuiControlState::FOCUSED:

        app->render->DrawRectangle(bounds, 255, 0, 0, 128, true, true);


       
        break;

    case GuiControlState::PRESSED:

        app->render->DrawRectangle(bounds, 255, 0, 0, 128, true, true);
       
        break;

    case GuiControlState::SELECTED:

        app->render->DrawRectangle(bounds, 255, 0, 0, 128, true, true);


        if (checked)
            checked = false;
        else if (!checked)
            checked = true;

        if (id == 8 || id == 11)
        {
            if (checked)
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
                app->sceneMenu->isFullScreenCheck = true;
            }
            else if (!checked)
            {
                SDL_SetWindowFullscreen(app->win->window, 0);
                app->sceneMenu->isFullScreenCheck = false;
            }
        }

      
        NotifyObserver();
        break;

    default:
        break;
    }
    
    return false;
}

bool GuiCheckBox::Draw(Render* render)
{

    // Draw the right button depending on state



    return false;
}

bool GuiCheckBox::DrawDebug(Render* render)
{
    int scale = app->win->GetScale();

    SDL_Rect drawBounds = SDL_Rect({ bounds.x * scale, bounds.y * scale, bounds.w * scale, bounds.h * scale });

    switch (state)
    {
    case GuiControlState::DISABLED:
        render->DrawRectangle(drawBounds, 255, 0, 0, 128, true, false);
        break;
    case GuiControlState::FOCUSED:
        render->DrawRectangle(drawBounds, 0, 255, 0, 128, true, false);
        break;
    case GuiControlState::NORMAL:
        render->DrawRectangle(drawBounds, 0, 0, 255, 128, true, false);
        break;
    case GuiControlState::PRESSED:
        render->DrawRectangle(drawBounds, 255, 255, 0, 128, true, false);
        break;
    case GuiControlState::SELECTED:
        render->DrawRectangle(drawBounds, 0, 255, 255, 128, true, false);
        break;
    }

    return true;
}