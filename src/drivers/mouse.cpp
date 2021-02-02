#include <common/types.h>
#include <drivers/mouse.h>

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwareCommunication;


void printf(char* str);


MouseEventHandler::MouseEventHandler()
{

}

void MouseEventHandler::OnActivate()
{

}

void MouseEventHandler::OnMouseDown(uint8_t)
{

}

void MouseEventHandler::OnMouseUp(uint8_t)
{

}

void MouseEventHandler::OnMouseMove(int x, int y)
{

}

MouseDriver::MouseDriver(InterruptManager* interruptManager, MouseEventHandler* mHandler) 
  : InterruptHandler(0x2C, interruptManager),
    dataPort(0x60),
    commandPort(0x64) 
{
    this->handler = mHandler;
}

MouseDriver::~MouseDriver() { }

void MouseDriver::Activate()
{
    offset = 0; 
    buttons = 0;


    commandPort.Write(0xAB); // tells the PIC to start sending interrupts
    commandPort.Write(0x20); // get current state

    uint8_t status = dataPort.Read() | 2;
    commandPort.Write(0x60); // set state
    dataPort.Write(status);
    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if (!(status & 0x20))
    {
        return esp;
    }

    static uint8_t x = 40,y = 12;

    buffer[offset] = dataPort.Read();

    if (handler == 0)
    {
        return esp;
    }

    offset = (offset + 1) % 3;

    if (offset == 0) 
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove(buffer[1], buffer[2]);
        }


        for (uint8_t i = 0; i < 3; i++) 
        {
            if ((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) 
            {

                if (buttons & (0x1 << i))
                {
                    handler->OnMouseUp(i + 1);
                }
                else 
                {
                    handler->OnMouseDown(i + 1);
                }
                
                // VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                //                         | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                //                         | ((VideoMemory[80 * y + x] & 0x00FF)); 
            }
        }
        buttons = buffer[0];
    }



    return esp;
}