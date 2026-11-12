#include "hid_service.h"

int getNumber(IOHIDDeviceRef device, CFStringRef key);
std::string cfStringToStdString(CFStringRef str);
std::string getString(IOHIDDeviceRef device, CFStringRef key);

#ifdef __APPLE__
IOHIDManagerRef hidManager = nullptr;
#endif

uint32_t usagePage;
uint32_t usage;
CFIndex input;

/**
 * this is used in Menu update to check when first input was made
 */
IOHIDDeviceRef device = nullptr;
std::vector<ControllerMapping> inputElements;
ControllerMapping currentIE;

// sets an interval of input val for specific pages
// process is done when inputs are stopped
/**
 * 1. Press GCC Button with cursor
 *  -Set button name for callback
 *  -MapControllerMenu::Update waits till device is set (first input)
 * 2. Supply inputs. Compares the highes Value (+ || -)
 *  -After 2 Seconds of no input, save input range
 * 3. Update will reset all communication variables
 */
void device_callback(void *context, IOReturn result, void *sender, IOHIDValueRef value){
    IOHIDElementRef element = IOHIDValueGetElement(value);

    usagePage = IOHIDElementGetUsagePage(element);
    usage = IOHIDElementGetUsage(element);
    input = IOHIDValueGetIntegerValue(value);

    // capture first input device and save default values
    // compare all following inputs against default
    if(UI_MapControllerMenu::button != "" && !device){
        for (auto it = inputElements.begin(); it != inputElements.end(); )
        {
            if (it->bName == UI_MapControllerMenu::button){
                it = inputElements.erase(it);
                std::cout << "delete" << std::endl;

            }
            else
                ++it;
        }

        device = reinterpret_cast<IOHIDDeviceRef>(sender);

        // assuming every device input val starts at 0
        // could read last input value and compare with initial
        currentIE = (ControllerMapping){0,0, usagePage, usage , 1, UI_MapControllerMenu::button};
    }


    if(!device){
        std::cout << "sender is not device or valid Pointer" << std::endl;
        return;
    }
    // post first input
    // compare values here

    if(usagePage != currentIE.uPage || usage != currentIE.usage){
        std::cout << "wrong button !" << std::endl;
        return;
    }   

    // feedback to gameloop menu
    // stall/reset timer when input
    UI_MapControllerMenu::start = SDL_GetTicks();


    if(abs(input) > abs(currentIE.secondVal)){
        currentIE.secondVal = input;
        currentIE.inputCount++;
    }

    std::cout << "Usage Page: " << usagePage << std::dec
              << " Usage: " << usage
              << " Value: " << input << " device: " <<  getNumber(device, CFSTR("bInterfaceNumber"))<<'\n';

}
bool containsIgnoreCase(const std::string& str, const std::string& search)
{
    return std::search(
        str.begin(), str.end(),
        search.begin(), search.end(),
        [](unsigned char a, unsigned char b)
        {
            return std::tolower(a) == std::tolower(b);
        }
    ) != str.end();
}
void readWholeProperties(){
    /*
        // display all properties (very long)
        io_service_t service = IOHIDDeviceGetService(device);
        CFMutableDictionaryRef properties = nullptr;
        if (service != IO_OBJECT_NULL )
        {
            std::cout << name << (containsIgnoreCase(name, "Mayflash")) << std::endl;
            IORegistryEntryCreateCFProperties(
                service,
                &properties,
                kCFAllocatorDefault,
                kNilOptions
            );


            if (properties)
            {

                //CFShow(properties);
                CFStringRef descR = CFCopyDescription(properties);

                if (descR)
                {
                    std::string desc = cfStringToStdString(descR);
                    std::cout << desc << std::endl;
                    if(true){
                        std::ofstream myfile("example.txt", std::ios::app);

                        if (myfile)
                        {
                            myfile << desc << "\n\n";
                            myfile.flush();
                        }
                        else
                        {
                            std::cerr << "Could not open example.txt\n";
                        }

                    }

                    ////std::cout << "CFString length: " << CFStringGetLength(descR) << '\n';
//std::cout << "Converted length: " << desc.size() << '\n';
//std::cout << "Description:\n" << desc << '\n';

                    CFRelease(descR);

                }

            }
            CFRelease(properties);

        }


*/
}
int getNumber(IOHIDDeviceRef device, CFStringRef key)
{
    CFTypeRef property = IOHIDDeviceGetProperty(device, key);

    if (!property || CFGetTypeID(property) != CFNumberGetTypeID())
        return -1;

    int value = 0;

    if (!CFNumberGetValue(
            static_cast<CFNumberRef>(property),
            kCFNumberIntType,
            &value))
        return -1;

    return value;
}

std::string cfStringToStdString(CFStringRef str)
{
    if (!str)
        return {};

    char buffer[16384];

    if (!CFStringGetCString(
            str, buffer, sizeof(buffer), kCFStringEncodingUTF8))
        return {};

    return buffer;
}

std::string getString(IOHIDDeviceRef device, CFStringRef key)
{
    CFTypeRef property = IOHIDDeviceGetProperty(device, key);

    if (!property || CFGetTypeID(property) != CFStringGetTypeID())
        return {};

    return cfStringToStdString(static_cast<CFStringRef>(property));
}
// find all HID Devices on MacOS using its IOKIT Framework
int findHIDDevice(std::string hidName){
    hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDManagerOptionNone);

    if (!hidManager)
        return 1;

    IOReturn result = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);

    if (result != kIOReturnSuccess)
    {
        std::cerr << "Could not open HID manager\n";
            std::cerr << "IOReturn: 0x" << std::hex << result << std::dec << "\n";

        CFRelease(hidManager);
        return 1;
    }

    // get ALL HID Devices
    IOHIDManagerSetDeviceMatching(hidManager, nullptr);

    std::cout << "IOHIDManagerOpen: 0x"
          << std::hex << static_cast<uint32_t>(result)
          << std::dec << std::endl;

    // the iterateable set
    CFSetRef deviceSet = IOHIDManagerCopyDevices(hidManager);

    if (!deviceSet)
    {
        std::cerr << "Could not open HID device set\n";
        CFRelease(hidManager);
        return 0;
    }
    std::cout << "Device set: "
          << (deviceSet ? "valid" : "null")
          << std::endl;


    std::cout << "Found "
            << CFSetGetCount(deviceSet)
            << " HID devices" << std::endl;
    

    CFIndex count = CFSetGetCount(deviceSet);
    std::vector<const void*> rawDevices(count);
    CFSetGetValues(deviceSet, rawDevices.data());

    std::cout << "Found " << count << " HID devices\n\n";
    int foundDevices = 0;
    for (const void* rawDevice : rawDevices)
    {
        IOHIDDeviceRef deviceRaw =
        static_cast<IOHIDDeviceRef>(const_cast<void*>(rawDevice));
        
        int vendorID  = getNumber(deviceRaw, CFSTR(kIOHIDVendorIDKey));
        int productID = getNumber(deviceRaw, CFSTR(kIOHIDProductIDKey));

        std::string name = getString(deviceRaw, CFSTR(kIOHIDProductKey));
        std::string manufacturer = getString(deviceRaw, CFSTR(kIOHIDManufacturerKey));
        std::string transport = getString(deviceRaw, CFSTR(kIOHIDTransportKey));

        int locationID = getNumber(deviceRaw, CFSTR(kIOHIDLocationIDKey));
        int usagePage = getNumber(deviceRaw, CFSTR(kIOHIDPrimaryUsagePageKey));
        int usage = getNumber(deviceRaw, CFSTR(kIOHIDPrimaryUsageKey));
        std::string serial = getString(deviceRaw, CFSTR(kIOHIDSerialNumberKey));

        // for an GameCube adapter, that has 4 Ports
        // identify them ranging 0-3 with this property
        auto interfaceNumber = getNumber(deviceRaw, CFSTR("bInterfaceNumber"));

        readWholeProperties();
        std::cout
            << "Name:              " << name << '\n'
            << "Manufacturer:      " << manufacturer << '\n'
            << "Transport:         " << transport << '\n'
            << "Vendor ID:         0x" << std::hex << vendorID << '\n'
            << "Product ID:        0x" << productID << '\n'
            << "Location ID:       0x" << locationID << '\n'
            << "Usage Page:        0x" << usagePage << '\n'
            << "Usage:             0x" << usage << '\n'
            << "Serial:            " << serial << '\n'
            << "int num:           " << interfaceNumber << std::endl
            << "-----------------------------\n";
        
        if(containsIgnoreCase(name, hidName)){
            std::cout << "found needed HID !" << std::endl;
            IOHIDDeviceRegisterInputValueCallback(deviceRaw, device_callback, nullptr);
            foundDevices++;
        }
    }
    if(foundDevices == 0){
        std::cout << "couldnt find the HID Device for: " << hidName << std::endl;
        CFRelease(deviceSet);
        CFRelease(hidManager);
        return 0;
    }



    IOHIDManagerScheduleWithRunLoop(
        hidManager,
        CFRunLoopGetCurrent(),
        kCFRunLoopDefaultMode
    );



    // 6. Release resources
    CFRelease(deviceSet);

    return 0;
}