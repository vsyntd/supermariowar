#include <IOKit/hid/IOHIDManager.h>
#include <vector>
#include <iostream>


void m(){
    auto ref = IOHIDManagerCreate(CFAllocatorGetDefault(), kIOHIDManagerOptionNone);
    IOReturn m = IOHIDManagerOpen(ref, kIOHIDOptionsTypeNone);
    CFSetRef deviceSet = IOHIDManagerCopyDevices(ref);
    CFIndex count = CFSetGetCount(deviceSet);

    std::vector<IOHIDDeviceRef> devices(count);

    for (IOHIDDeviceRef device : devices)
    {
        auto getNumber = [&](CFStringRef key) -> int
        {
            CFTypeRef property = IOHIDDeviceGetProperty(device, key);

            if (!property || CFGetTypeID(property) != CFNumberGetTypeID())
                return -1;

            int value = 0;

            CFNumberGetValue(
                static_cast<CFNumberRef>(property),
                kCFNumberIntType,
                &value
            );

            return value;
        };

        auto getString = [&](CFStringRef key) -> std::string
        {
            CFTypeRef property = IOHIDDeviceGetProperty(device, key);

            if (!property || CFGetTypeID(property) != CFStringGetTypeID())
                return {};

            char buffer[256];

            if (!CFStringGetCString(
                    static_cast<CFStringRef>(property),
                    buffer,
                    sizeof(buffer),
                    kCFStringEncodingUTF8))
                return {};

            return buffer;
        };

        int vendorID  = getNumber(CFSTR(kIOHIDVendorIDKey));
        int productID = getNumber(CFSTR(kIOHIDProductIDKey));

        std::string name = getString(CFSTR(kIOHIDProductKey));
        std::string manufacturer = getString(CFSTR(kIOHIDManufacturerKey));
        std::string transport = getString(CFSTR(kIOHIDTransportKey));

        std::cout
            << "Name:         " << name << '\n'
            << "Manufacturer: " << manufacturer << '\n'
            << "Transport:    " << transport << '\n'
            << "Vendor ID:    0x" << std::hex << vendorID << '\n'
            << "Product ID:   0x" << productID << std::dec << '\n'
            << "-----------------------------\n";
    }

}