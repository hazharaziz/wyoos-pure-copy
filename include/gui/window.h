#ifndef __MYOS__GUI__WINDOW__H
#define __MYOS__GUI__WINDOW__H

#include <common/types.h>
#include <gui/widget.h>
#include <drivers/mouse.h>


namespace myos 
{
    namespace gui
    {
        class Window : public CompositeWidget
        {
            protected:
                bool Dragging; 

            public:
                Window(Widget* parent, myos::common::int32_t x,
                    myos::common::int32_t y, myos::common::int32_t w,
                    myos::common::int32_t h, myos::common::uint8_t r,
                    myos::common::uint8_t g, myos::common::uint8_t b);
                ~Window();

                void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                void OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY);                 


        };
    }
}


#endif