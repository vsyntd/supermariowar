#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL_rect.h>
#include <IOKit/hid/IOHIDManager.h>

// to avoid circle include 

class MI_Image;

enum class InputType{
    BUTTON,
    JOYSTICK
};
enum class ID : int{
    P1, P2, P3, P4, BG, PORT, CONTROLLER, CURSOR
};
constexpr int toInt(ID id)
{
    return static_cast<int>(id);
}

typedef struct ControllerMapping{
    int initVal, secondVal;
    uint32_t uPage, usage, inputCount;
    std::string bName;
    bool isJoystick;
}ControllerMapping;


struct ControllerElement
{
    MI_Image* image;
    SDL_Rect hitbox;
};

using ControllerBag = std::unordered_map<std::string, ControllerElement>;

int getNumber(IOHIDDeviceRef device, CFStringRef key);