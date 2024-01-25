#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "../GuiControl.h"

#include "../Point.h"
#include "../SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char* tex);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw(Render* render);
    bool DrawDebug(Render* render);

    // GuiCheckBox specific properties
    // Maybe some animation properties for state change?
    bool checked = false;

private:

    

    bool playFxOnce = true;
};

#endif // __GUICHECKBOX_H__
