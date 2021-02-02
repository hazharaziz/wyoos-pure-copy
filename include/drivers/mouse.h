#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/port.h>
#include <drivers/driver.h>

namespace myos 
{
    namespace drivers 
    {
        class MouseEventHandler
        {
            public:
                MouseEventHandler();
                virtual void OnActivate();
                virtual void OnMouseDown(myos::common::uint8_t button);
                virtual void OnMouseUp(myos::common::uint8_t button);
                virtual void OnMouseMove(int xOffset, int yOffset);
        };


        class MouseDriver : public myos::hardwareCommunication::InterruptHandler, public Driver
        {
            myos::hardwareCommunication::Port8Bit dataPort;
            myos::hardwareCommunication::Port8Bit commandPort;

            MouseEventHandler* handler;

            myos::common::uint8_t buffer[3];
            myos::common::uint8_t offset;
            myos::common::uint8_t buttons;

            public:
                MouseDriver(myos::hardwareCommunication::InterruptManager* interruptManager, MouseEventHandler* handler);
                ~MouseDriver();
                virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
                virtual void Activate();
        };

    }
}


#endif