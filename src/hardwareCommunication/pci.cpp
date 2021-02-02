#include <common/types.h>
#include <hardwareCommunication/pci.h>
#include <drivers/driver.h>

using namespace myos::common;
using namespace myos::hardwareCommunication;
using namespace myos::drivers;

void printf(char* str);
void printfHex(uint8_t);

PCIDeviecDescriptor::PCIDeviecDescriptor()
{

}

PCIDeviecDescriptor::~PCIDeviecDescriptor()
{

}


PCIController::PCIController()
   : dataPort(0xCFC),
     commandPort(0xCF8)
{

}

PCIController::~PCIController()
{

}


uint32_t PCIController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset)
{
    uint32_t id = 0x1 << 31
     | ((bus & 0xFF) << 16)
     | ((device & 0x1F) << 11)
     | ((function & 0x07) << 8)
     | ((registerOffset & 0xFC));

    commandPort.Write(id);
    uint32_t result = dataPort.Read();

    
    return result >> (8 * (registerOffset % 4));
}


void PCIController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, uint32_t value)
{
    uint32_t id = 0x1 << 31
     | ((bus & 0xFF) << 16)
     | ((device & 0x1F) << 11)
     | ((function & 0x07) << 8)
     | ((registerOffset & 0xFC));

    commandPort.Write(id);
    dataPort.Write(value);
}

bool PCIController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7); // this address specifies if the device has functions on it 
}

void PCIController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts)
{
    for (int bus = 0; bus < 8; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < numFunctions; function++)
            {
                PCIDeviecDescriptor dev = GetDeviceDescriptor(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                {
                    continue;
                }

                for (int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if (bar.address && (bar.type == IO))
                    {
                        dev.portBase = (uint32_t)bar.address;
                    }

                    Driver* driver = GetDriver(dev, interrupts);
                    if (driver != 0)
                    {
                        driverManager->AddDriver(driver);
                    }
                }

                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);

                printf(" = VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);

                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

BaseAddressRegister PCIController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;

    uint32_t headerType = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headerType);
    if (bar >= maxBARs)
    {
        return result;
    }


    uint32_t bar_value = Read(bus, device, function, 0x10 + (4 * bar));
    result.type = (bar_value & 0x1) ? IO : MemoryMapping;
    uint32_t temp;

    if (result.type == MemoryMapping)
    {
        
        result.prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
        switch ((bar_value >> 1) & 0x3)
        {   
            case 0: // 32 bit mode

                break;
            case 1: // 20 bit mode

                break;
            case 2: // 64 bit mode

                break;
            default:

                break;

        }
    }
    else 
    {
        result.address == (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }


    return result;
}

Driver* PCIController::GetDriver(PCIDeviecDescriptor device, InterruptManager* interrupts)
{
    switch (device.vendor_id)
    {
        case 0x1022:
            switch (device.device_id)
            {
                case 0x2000: // am 79c973     
                    printf("AMD am79c973");               
                    break;
                
                default:
                    break;
            }
            break;
        
        case 0x8086: // Intel

            break;
        
        default:
            break;
    }

    switch (device.class_id)   
    {
        case 0x03: // graphics
            switch (device.subclass_id)    
            {
                case 0x00: // VGA 
                    printf("VGA");               
                    break;
                
                default:
                    break;
            }
            break;
        
        default:
            break;
    }

    return 0;
}

PCIDeviecDescriptor PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDeviecDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);



    return result;
}