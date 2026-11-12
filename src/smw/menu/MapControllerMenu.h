#pragma once
#include "uimenu.h"
#include "ResourceManager.h"
#include "Game.h"
#include <unordered_map>
#include <string>
#include "menu/ControllerTypes.h"
#include "ui/MI_Text.h"
#include "ui/MI_TextField.h"
#include "ui/MI_Cursor.h"
#include "ui/MI_Button.h"
#include <IOKit/hid/IOHIDManager.h>
#include <chrono>


extern CResourceManager* rm;
extern IOHIDDeviceRef device;
extern ControllerMapping currentIE;
extern std::vector<ControllerMapping> inputElements;

class UI_MapControllerMenu : public UI_Menu{

public:
    UI_MapControllerMenu();
    bool isInExplainMode = true;
    inline static std::string button = "";
    inline static UInt64 start;

    MI_Image* bg;
    /**
     *  To debug hitbox coordinates get a pointer to the Menu like this for example:
     *     auto mp = dynamic_cast<UI_MapControllerMenu*>(mCurrentMenu);
            if(mp ){
                mp->DebugHitbox(screen);
            }
     */
    void DebugHitbox(SDL_Surface* screen);
    void Update();
    void InitMenu();
    ControllerBag controllerImages;


private:
    void AddImagesToBag(int imageX, int imageY, int spriteSize);
    void SetBackground(int bgX, int bgY, int w, int h);
    void SetDescription(int bgX, int bgY, int w, int h);
    MI_Cursor* cursor = nullptr;
    MI_Image* prevVis = nullptr;
    std::string hidName = "";
    bool deactivateGuide = true; 
};