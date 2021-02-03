#ifndef __MYOS__GUI__DESKTOP__H
#define __MYOS__GUI__DESKTOP__H

#include <common/types.h>
#include <common/graphicsContext.h>
#include <gui/widget.h>
#include <drivers/mouse.h>

namespace myos 
{
    namespace gui
    {
        class Desktop : public CompositeWidget, public myos::drivers::MouseEventHandler
        {
            protected:
                myos::common::int32_t MouseX;
                myos::common::int32_t MouseY;

            public:
                Desktop(myos::common::int32_t w, myos::common::int32_t h, myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
                ~Desktop();

                void Draw(myos::common::GraphicsContext* gc);
                void OnMouseDown(myos::common::uint8_t button);
                void OnMouseUp(myos::common::uint8_t button);
                void OnMouseMove(int x, int y);
        };
    }
}


#endif