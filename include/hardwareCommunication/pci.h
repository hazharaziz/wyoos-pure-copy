#ifndef __MYOS__HARDWARECOMMUNICATION__PCI__H
#define __MYOS__HARDWARECOMMUNICATION__PCI__H

#include <hardwareCommunication/port.h>
#include <common/types.h>
#include <hardwareCommunication/interrupts.h>
#include <drivers/driver.h>

namespace myos
{
    namespace hardwareCommunication 
    {

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            IO = 1  
        };

        class BaseAddressRegister
        {
            public:
                bool prefetchable;
                myos::common::uint8_t* address;
                myos::common::uint32_t size;
                BaseAddressRegisterType type;
        };

        class PCIDeviecDescriptor 
        {
            public:
                myos::common::uint32_t portBase;
                myos::common::uint32_t interrupt;
                myos::common::uint16_t bus;
                myos::common::uint16_t device;
                myos::common::uint16_t function;

                myos::common::uint16_t vendor_id;
                myos::common::uint16_t device_id;

                myos::common::uint8_t class_id; 
                myos::common::uint8_t subclass_id;
                myos::common::uint8_t interface_id;
                myos::common::uint8_t revision;

                PCIDeviecDescriptor();
                ~PCIDeviecDescriptor();
        };


        class PCIController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;

            public:
                PCIController();
                ~PCIController();
                
                myos::common::uint32_t Read(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registerOffset);
                void Write(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registerOffset, myos::common::uint32_t value);
                bool DeviceHasFunctions(myos::common::uint16_t bus, myos::common::uint16_t device);
                
                void SelectDrivers(myos::drivers::DriverManager* driverManager, myos::hardwareCommunication::InterruptManager* interrupts);
                myos::drivers::Driver* GetDriver(PCIDeviecDescriptor device, myos::hardwareCommunication::InterruptManager* interrupts);
                PCIDeviecDescriptor GetDeviceDescriptor(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function);
                BaseAddressRegister GetBaseAddressRegister(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint16_t bar);

        };

    }   
}

#endif