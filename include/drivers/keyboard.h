#ifndef __MYOS__DRIVERS__KEYBOARD_H
#define __MYOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>


namespace myos 
{
    namespace drivers 
    {
        class KeyboardEventHandler
        {
            public:
                KeyboardEventHandler();

                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public myos::hardwareCommunication::InterruptHandler, public Driver
        {
            myos::hardwareCommunication::Port8Bit dataPort;
            myos::hardwareCommunication::Port8Bit commandPort;
            KeyboardEventHandler* handler;

            public:
                KeyboardDriver(myos::hardwareCommunication::InterruptManager* interruptManager, KeyboardEventHandler* handler);
                ~KeyboardDriver();
                virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
                virtual void Activate();
        };

    }
}

#endif