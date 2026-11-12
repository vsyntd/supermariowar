#include "MI_Cursor.h"
#include "menu/MapControllerMenu.h"

void MI_Cursor::Update()
{

    float mouseX = -1, mouseY = -1;

    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if(mouseX == -1 || mouseY == -1)
        return;

    short x,y ,w, h;

    GetPositionAndSize(x, y, w, h );
    this->m_pos.x = mouseX - w / 2.f;
    this->m_pos.y = mouseY - h / 2.f;

    // since UI_MapControllerMenu isnt called in the main loop...
    auto p = dynamic_cast<UI_MapControllerMenu*>(this->m_parentMenu);
    if(p){
        p->Update();
    }


}


