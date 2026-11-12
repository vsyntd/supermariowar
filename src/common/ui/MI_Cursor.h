#pragma once

#include "ui/MI_Image.h"
#include "menu/ControllerTypes.h"
#include <SDL3/SDL_mouse.h>

class UI_MapControllerMenu;

class MI_Cursor : public MI_Image
{
public:
    MI_Cursor(
        gfxSprite* nspr,
        short x, short y,
        short srcx, short srcy,
        short w, short h,
        short numxframes, short numyframes,
        short speed, int id = 0
    )
        : MI_Image(nspr, x, y, srcx, srcy, w, h,
                   numxframes, numyframes, speed, id)

    {
    }


    void Update() override;
};