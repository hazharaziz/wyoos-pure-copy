#include <common/types.h>
#include <gdt.h>
#include <multitasking.h>
#include <hardwareCommunication/interrupts.h>
#include <hardwareCommunication/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/driver.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/widget.h>
#include <gui/window.h>

// #define GRAPHICSMODE

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwareCommunication;
using namespace myos::gui;

void printf(char* str) 
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0; 

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;
            
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if (x >= 80) 
        {
            y++;
            x = 0;
        }

        if (y >= 25) 
        {
            for (y = 0; y < 25; y++) 
            {
                for (x = 0; x < 80; x++) 
                {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' '/*str[i]*/;
                }
            }

            x = 0;
            y = 0;
        }
    }
}


void printfHex(uint8_t key)
{
    char* foo = "0x00";
    char* hex = "0123456789ABCDEF";
    foo[2] = hex[(key >> 4) & 0x0F];
    foo[3] = hex[key & 0x0F];
    printf(foo); 
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
    public:
        void OnKeyDown(char ch)
        {
            char* foo = " ";
            foo[0] = ch;
            printf(foo);
        }
};


class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
    public:

        MouseToConsole()
        {
            x = 40;
            y = 12;
            uint16_t* VideoMemory = (uint16_t*)0xb8000;
            VideoMemory[80 * 12 + 40] = ((VideoMemory[80 * 12 + 40] & 0xF000) >> 4)
                            | ((VideoMemory[880 * 12 + 40] & 0x0F00) << 4)
                            | (VideoMemory[80 * 12 + 40] & 0x00FF); 
        }

        void OnMouseMove(int xOffset, int yOffset)
        {
            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                            | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                            | ((VideoMemory[80 * y + x] & 0x00FF)); 

    
            x += xOffset;

            if (x < 0) x = 0;
            if (x >= 80) x = 79;

            y -= yOffset; 

            if (y < 0) y = 0;
            if (y >= 25) y = 24;

            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                                    | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                                    | ((VideoMemory[80 * y + x] & 0x00FF)); 

        }
};

void taskA()
{
    while(true)
        printf("A");
}
void taskB()
{
    while(true)
        printf("B");
}

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()  
{
    for (constructor* i = &start_ctors; i != &end_ctors; i++) 
    {
        (*i)();
    }
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*magicnumber*/) 
{
    printf("This is my operating system!\n");

    GlobalDescriptorTable gdt;

    TaskManager taskManager;
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    printf("Initializing drivers, Stage 1\n");

    #ifdef GRAPHICSMODE
        Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
    #endif

    DriverManager driverManager;

    #ifdef GRAPHICSMODE
        KeyboardDriver keyboard(&interrupts, &desktop);
    #else
        PrintfKeyboardEventHandler keyboardHandler;
        KeyboardDriver keyboard(&interrupts, &keyboardHandler);
    #endif
    driverManager.AddDriver(&keyboard);

    #ifdef GRAPHICSMODE
        MouseDriver mouse(&interrupts, &desktop); 
    #else 
        MouseToConsole mouseHandler;
        MouseDriver mouse(&interrupts, &mouseHandler); 
    #endif
    driverManager.AddDriver(&mouse);

    PCIController pciController;
    pciController.SelectDrivers(&driverManager, &interrupts);


    VGA vga;


    printf("Activating all drivers!, Stage 2\n");

    driverManager.ActivateAll(); 

    printf("Activating the interrupts!, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320, 200, 8);
        Window window1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
        desktop.AddChild(&window1);
        Window window2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
        desktop.AddChild(&window2);
    #endif


    interrupts.Activate(); 


    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}   