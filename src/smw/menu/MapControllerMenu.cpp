#include "menu/MapControllerMenu.h"


// cursor kann weg
//sdl übernimmt click events hier
UI_MapControllerMenu::UI_MapControllerMenu()
{
    const int spriteSize = rm->spr_game_cube_controller.getWidth() / 4;

    const int imageW = spriteSize;
    const int imageH = spriteSize;

    const int imageXCentered = (App::screenWidth - imageW) / 2;
    const int imageYCentered = (App::screenHeight - imageH) / 2 - 3;

    AddImagesToBag(imageXCentered, imageYCentered, spriteSize);

    SDL_SetSurfaceAlphaMod(rm->spr_gcc_port.getSurface(), 170); // 0–255

    auto port = new MI_Image(
        &rm->spr_gcc_port,
        (App::screenWidth - rm->spr_gcc_port.getWidth()) / 2 - App::screenWidth * 0.23f,
        (App::screenHeight - rm->spr_gcc_port.getHeight()) / 2- App::screenHeight * 0.33f,
        0, 0,
        rm->spr_gcc_port.getWidth(),
        rm->spr_gcc_port.getHeight(),
        1, 1, 0, toInt(ID::PORT)
    );
    port->SetScale(true, 0.34f);
    port->setVisible(false);
    AddNonControl(port);

    auto pHighlight = new MI_Image(
        &rm->spr_gcc_port_highlight1,
        (App::screenWidth - rm->spr_gcc_port.getWidth()) / 2 - App::screenWidth * 0.23f,
        (App::screenHeight - rm->spr_gcc_port.getHeight()) / 2- App::screenHeight * 0.33f,
        0, 0,
        rm->spr_gcc_port.getWidth(),
        rm->spr_gcc_port.getHeight(),
        1, 1, 0, toInt(ID::P1)
    );
    pHighlight->SetScale(true, 0.34f);
    pHighlight->setVisible(false);
    AddNonControl(pHighlight);
    auto pHighlight2 = new MI_Image(
        &rm->spr_gcc_port_highlight2,
        (App::screenWidth - rm->spr_gcc_port.getWidth()) / 2 - App::screenWidth * 0.23f,
        (App::screenHeight - rm->spr_gcc_port.getHeight()) / 2- App::screenHeight * 0.33f,
        0, 0,
        rm->spr_gcc_port.getWidth(),
        rm->spr_gcc_port.getHeight(),
        1, 1, 0, toInt(ID::P2)
    );
    pHighlight2->SetScale(true, 0.34f);
    pHighlight2->setVisible(false);
    AddNonControl(pHighlight2);
    auto pHighlight3 = new MI_Image(
        &rm->spr_gcc_port_highlight3,
        (App::screenWidth - rm->spr_gcc_port.getWidth()) / 2 - App::screenWidth * 0.23f,
        (App::screenHeight - rm->spr_gcc_port.getHeight()) / 2- App::screenHeight * 0.33f,
        0, 0,
        rm->spr_gcc_port.getWidth(),
        rm->spr_gcc_port.getHeight(),
        1, 1, 0, toInt(ID::P3)
    );
    pHighlight3->SetScale(true, 0.34f);
    pHighlight3->setVisible(false);
    AddNonControl(pHighlight3);
    auto pHighlight4 = new MI_Image(
        &rm->spr_gcc_port_highlight4,
        (App::screenWidth - rm->spr_gcc_port.getWidth()) / 2 - App::screenWidth * 0.23f,
        (App::screenHeight - rm->spr_gcc_port.getHeight()) / 2- App::screenHeight * 0.33f,
        0, 0,
        rm->spr_gcc_port.getWidth(),
        rm->spr_gcc_port.getHeight(),
        1, 1, 0, toInt(ID::P4)
    );
    pHighlight4->SetScale(true, 0.34f);
    pHighlight4->setVisible(false);
    AddNonControl(pHighlight4);

    



    // 33 32

    // Scale all controller images
    for (auto& [name, element] : controllerImages)
    {
        element.image->SetScale(true, 0.35f);
        element.image->setVisible(false);
        AddNonControl(element.image);
    }
    // transform all hitboxes one time
    for(auto& e : controllerImages){
        auto& buttonName = e.first;
        auto& imgHitbox = e.second;
        auto& img = imgHitbox.image;
        auto& hb = imgHitbox.hitbox;

        // hb.x += imageX;
        // hb.y += imageYCentered;

        float scaleValImg = img->GetScale();

        // transform all hitboxes relative to the GameCube image
        // e.g. if 70% scaling, apply 15% for x and 15% for w (inverseScaleHalf)
        // therefore 70% is centered

        float inverseScaleHalf = (1.0f - scaleValImg)/2.f;
        float scaleXBaseImage = inverseScaleHalf * imageW;
        float scaleXHitbox = hb.x * scaleValImg;

        hb.x = imageXCentered + scaleXBaseImage + scaleXHitbox;

        float scaleYBaseImage = inverseScaleHalf * imageH;
        float scaleYHitbox = hb.y * scaleValImg;

        hb.y = imageYCentered + scaleYBaseImage + scaleYHitbox;

        hb.w *=  scaleValImg;
        hb.h *=  scaleValImg;
    }

    const int bgW = rm->menu_dialog.getWidth();
    const int bgH = rm->menu_dialog.getHeight();

    const int bgX = (App::screenWidth - bgW) / 2;
    const int bgY = (App::screenHeight - bgH) / 2;
    const int w = App::screenWidth - bgX * 2;
    const int h = App::screenWidth - bgY * 2;

    SetBackground(bgX, bgY, w, h);
    SetDescription(bgX, bgY, w, h);

    auto cursor = new MI_Cursor(
        &rm->spr_cursor,
        0, 0,
        0, 0,
        rm->spr_cursor.getWidth(),
        rm->spr_cursor.getHeight(),
        1, 1, 0, toInt(ID::CURSOR)
    );

    cursor->SetScale(true, 1.3f);
    AddNonControl(cursor);

    SetCancelCode(MENU_CODE_TO_MAIN_MENU);
}

void UI_MapControllerMenu::DebugHitbox(SDL_Surface* screen)
{
    if(!screen)
        return;

    for (auto& e : controllerImages){

        Uint32 red = SDL_MapSurfaceRGB(screen, 255, 0, 0);
        auto black = SDL_MapSurfaceRGB(screen, 0,0,0);

        SDL_FillSurfaceRect(screen, &e.second.hitbox, red); 
        SDL_Rect a = {          
            e.second.hitbox.x + 3,
            e.second.hitbox.y + 3,
            e.second.hitbox.w - 6,
            e.second.hitbox.h -6,
        };

        SDL_FillSurfaceRect(screen, &a, black); 
    }
}
void UI_MapControllerMenu::Update()
{
    static std::string prevImgName = "";

    if(isInExplainMode)
        return;

    if(deactivateGuide){
        for(auto& e : controls){
            auto text = dynamic_cast<MI_Text*>(e.get());
            if(text)
                text->setVisible(false);
            auto b = dynamic_cast<MI_Button*>(e.get());
            if(b)
                b->setVisible(false);
            auto img = dynamic_cast<MI_Image*>(e.get());
            if(img && img->id == toInt(ID::BG))
                img->setVisible(false);
            if(img && img->id == toInt(ID::PORT))
                img->setVisible(true);
            auto tf = dynamic_cast<MI_TextField*>(e.get());
            if(tf)
                tf->setVisible(false);

        }  
        deactivateGuide = false;     
        controllerImages.at("Normal").image->setVisible(true);
    }
    static bool pressed = false;
    for(auto& e : controllerImages){
        auto buttonName = e.first;
        auto& imgHitbox = e.second;
        auto& img = imgHitbox.image;
        auto& hb = imgHitbox.hitbox;

        float mouseX = -1, mouseY = -1;

        SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point point = {mouseX , mouseY};

        // change sprite once entering (visual)
        if (SDL_PointInRect(&point, &hb) && prevImgName != buttonName)
        {
            std::cout << "is in: " << buttonName << std::endl;
            prevVis->setVisible(false);
            prevImgName = buttonName;
            img->setVisible(true);
            prevVis = img;
        }


        if(SDL_PointInRect(&point, &hb)){

            // set button and wait for hid callback to read first input
            // timer is stalled as long as there is no input
            // you can change button before you do your first input
            bool pingCallback_BlockWhileInUse = buttons & SDL_BUTTON_LMASK && !pressed && !device;
            if (pingCallback_BlockWhileInUse)
            {
                pressed = true;
                button = buttonName;
                start = SDL_GetTicks();
                std::cout << "click" << std::endl;
                break; 
            }
            else if(!(buttons & SDL_BUTTON_LMASK))
                pressed = false;

            bool timoutSaveInputRange = button != "" && SDL_GetTicks() - start > 2000; 
            if(timoutSaveInputRange){
                // done inputting 
                button = "";
                std::cout << "done input " <<
                 currentIE.initVal << " "  
                 << currentIE.secondVal << " " << 
                 currentIE.uPage << " " << currentIE.usage << std::endl;
                 currentIE.isJoystick = currentIE.inputCount > 1;
                inputElements.push_back(currentIE);

                // reset capture device mechanism in callback
                device = nullptr;
            }
        }

        if (buttons & SDL_BUTTON_RMASK)
        {
            // Right mouse button is currently held
        }
    }

    // reset timer, wait for first input in callback
    if(!device)
        start = SDL_GetTicks();
    else{
        int interfaceNumber = getNumber(device, CFSTR("bInterfaceNumber"));

        // set correct 
        for(auto& e : controls){
            auto pi = dynamic_cast<MI_Image*>(e.get());

            if(pi && pi->id == interfaceNumber){
                pi->setVisible(true);
            }else if(pi && pi->id < 4 && pi->id != interfaceNumber)
                pi->setVisible(false);
                
        }  
    }

}

void UI_MapControllerMenu::InitMenu()
{
    prevVis = controllerImages.at("Normal").image;
    prevVis->setVisible(false);

    for(auto& e : controls){
        auto text = dynamic_cast<MI_Text*>(e.get());
        if(text)
            text->setVisible(true);
        auto b = dynamic_cast<MI_Button*>(e.get());
        if(b)
            b->setVisible(true);
        auto bg = dynamic_cast<MI_Image*>(e.get());
        if(bg && bg->GetScale() == 0.9f)
            bg->setVisible(true);
        auto tf = dynamic_cast<MI_TextField*>(e.get());
        if(tf){
            tf->setVisible(true);
            setInitialFocus(tf);
        }
        auto pi = dynamic_cast<MI_Image*>(e.get());
        if(pi && pi->GetScale() == 0.34f)
            pi->setVisible(false);
    }  
    isInExplainMode = true;
    deactivateGuide = true;  
    controllerImages.at("Normal").image->setVisible(false);
}
/**
 * Connect the Buttonhitbox info with its corresponding name
 * The MI_Cursor class will handle Mouse inputs to these images
 */
void UI_MapControllerMenu::AddImagesToBag(int imageX, int imageY, int spriteSize)
{
    auto addControllerElement =
        [&](const std::string& name, int sourceX, int sourceY,
            SDL_Rect hitbox)
    {
        controllerImages.emplace(
            name,
            ControllerElement{
                new MI_Image(
                    &rm->spr_game_cube_controller,
                    imageX, imageY,
                    sourceX, sourceY,
                    spriteSize, spriteSize,
                    1, 1, 0, toInt(ID::CONTROLLER)
                ),
                hitbox
            }
        );
    };
    // Row 1
    addControllerElement("Start", 0, 0,
        {532, 457, 136, 121});

    addControllerElement("LS", 1200, 0,
        {190, 265, 295, 133});

    addControllerElement("RS", 2400, 0,
        {754, 256, 274, 118});

    addControllerElement("X", 3600, 0,
        {913, 397, 116, 185});

    // Row 2
    addControllerElement("Y", 0, 1200,
        {751, 379, 148, 79});

    addControllerElement("SL", 1200, 1200,
        {210, 465, 99, 66});

    addControllerElement("SD", 2400, 1200,
        {312, 526, 64, 91});

    addControllerElement("SR", 3600, 1200,
        {376, 472, 105, 55});

    // Row 3
    addControllerElement("SU", 0, 2400,
        {311, 401, 69, 70});

    addControllerElement("DD", 1200, 2400,
        {440, 727, 56, 83});

    addControllerElement("DR", 2400, 2400,
        {480, 673, 89, 52});

    addControllerElement("DL", 3600, 2400,
        {363, 660, 73, 84});

    // Row 4
    addControllerElement("DU", 0, 3600,
        {436, 590, 60, 82});

    addControllerElement("B", 1200, 3600,
        {727, 513, 72, 104});

    addControllerElement("A", 2400, 3600,
        {800, 460, 111, 130});

            // Base controller image (no hitbox)
    controllerImages.emplace(
        "Normal",
        ControllerElement{
            new MI_Image(
                &rm->spr_game_cube_controller,
                imageX, imageY,
                3600, 3600,
                spriteSize, spriteSize,
                1, 1, 0, toInt(ID::CONTROLLER)
            ),
            {0, 0, 0, 0}
        }
    );

}

void UI_MapControllerMenu::SetBackground(int bgX, int bgY, int w, int h)
{
    bg = new MI_Image(
        &rm->menu_dialog,
        bgX, bgY,
        0, 0,
        w,
        h,
        1, 1, 0, toInt(ID::BG)
    );
    bg->SetScale(true, 0.9f);
    bg->setVisible(true);

    AddNonControl(bg);
}

void UI_MapControllerMenu::SetDescription(int bgX, int bgY, int w, int h)
{
    auto titel = new MI_Text("How to map your Controller", 
        bgX + w* 0.1f, 
        bgY + h* 0.1f, 
        w*0.8f,
        true
    );

    std::vector<std::string> lines;

    lines.push_back("You can map your Controller to a");
    lines.push_back("virtual GameCube one, if your PC");
    lines.push_back("can't find yours. All you need is the");
    lines.push_back("Name of said Adapter/Controller.");
    lines.push_back("Press Enter and type in");
    lines.push_back("the Name of your Product");

    float yOffset = 0;
    for(auto& line : lines){
        auto description = new MI_Text(
            line,
            bgX + w * 0.1f,
            bgY + h * (0.2f + yOffset),
            w ,
            true
        );
        yOffset += 0.04f;

        description->setVisible(true);
        AddNonControl(description);
    }

    auto search = new MI_TextField(&rm->menu_plain_field, 
        bgX + w * 0.1f,
        bgY + h * (0.2f + yOffset + 0.04f),
        "Search",
        w *0.8f,
        100
    );

    search->setVisible(true);
    setInitialFocus(search);
    search->SetData(hidName, 127);

    auto next = new MI_Button(&rm->spr_selectfield, bgX + w * 0.7f, bgY + h * 0.7f * 0.9f, "Next", 80);
    next->setVisible(true);
    next->SetCode(MENU_CODE_MAP_CONTROLLER_SKIP_DESCRIPTION);
    search->SetAutoModify(true);
    AddControl(search,next, next, next, next);
    AddControl(next, search, search, search, search);
    titel->setVisible(true);
    AddNonControl(titel);
 
}
