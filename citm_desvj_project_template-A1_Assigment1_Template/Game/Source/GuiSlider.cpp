#include "GuiSlider.h"
#include "../App.h"
#include "../Scene.h"
#include "../GuiManager.h"
#include "../Audio.h"
#include "../Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex, SDL_Texture* tex2) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->unit = bounds.w / 100.0f;
    this->text = text;
    this->texture = tex;
    this->texture2 = tex2;
    if (id == 1)
    {
        //value = app->guimanager->musicVolume;
    }
    else if (id == 2)
    {
        //value = app->guimanager->fxVolume;
    }

    value = round(value);
    sliderPosx = ((value * unit) + bounds.x) - unit;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
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

            unit = bounds.w / 100.0f;
            value = (mouseX - bounds.x) / unit;
            value = round(value);

            value2 = (sliderPosx - bounds.x) / unit;
            value2 = round(value2);


            if (value2 < 2)
                value2 = 0;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                sliderPosx = ((value * unit) + bounds.x) - unit - 5;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                state = GuiControlState::SELECTED;
            }
        }
        else state = GuiControlState::NORMAL;

        switch (state)
        {
        case GuiControlState::DISABLED:
            break;

        case GuiControlState::NORMAL:
            if (sliderPosx - bounds.x >= 0)
                app->render->DrawRectangle({ bounds.x + 1,bounds.y + 2, sliderPosx - bounds.x, bounds.h }, 95, 205, 228, 255, true, true);

            app->render->DrawTexture(texture, bounds.x, bounds.y);
            app->render->DrawTexture(texture2, sliderPosx, bounds.y - 1);
            break;

        case GuiControlState::FOCUSED:

            if (sliderPosx - bounds.x >= 0)
                app->render->DrawRectangle({ bounds.x + 1,bounds.y + 2, sliderPosx - bounds.x, bounds.h }, 95, 205, 228, 255, true, true);

            app->render->DrawTexture(texture, bounds.x, bounds.y);
            app->render->DrawTexture(texture2, sliderPosx, bounds.y - 1);
            break;

        case GuiControlState::PRESSED:

            if (sliderPosx - bounds.x >= 0)
                app->render->DrawRectangle({ bounds.x + 1,bounds.y + 2, sliderPosx - bounds.x,bounds.h }, 95, 205, 228, 255, true, true);

            app->render->DrawTexture(texture, bounds.x, bounds.y);
            app->render->DrawTexture(texture2, sliderPosx, bounds.y - 1);
           
            app->scene->volume = value2;
            break;

        case GuiControlState::SELECTED:
            if (id == 2)

            if (sliderPosx - bounds.x >= 0)
                app->render->DrawRectangle({ bounds.x + 1,bounds.y + 2, sliderPosx - bounds.x,bounds.h }, 95, 205, 228, 255, true, true);

            app->render->DrawTexture(texture, bounds.x, bounds.y);
            app->render->DrawTexture(texture2, sliderPosx, bounds.y - 1);
            
            NotifyObserver();
            break;

        default:
            break;
        }
      
    }

    app->scene->volume = value2;
    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state



    return false;
}

bool GuiSlider::DrawDebug(Render* render)
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