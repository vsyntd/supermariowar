#pragma once
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/IOKitLib.h>
#include <iostream>
#include "menu/MapControllerMenu.h"
#include "menu/ControllerTypes.h"

#ifdef __APPLE__
extern IOHIDManagerRef hidManager;

int findHIDDevice(std::string hidName);

#endif

extern std::string button;