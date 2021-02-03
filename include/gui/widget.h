#ifndef __MYOS__GUI__WIDGET__H
#define __MYOS__GUI__WIDGET__H

#include <common/types.h>
#include <common/graphicsContext.h>
#include <drivers/keyboard.h>

namespace myos 
{
    namespace gui
    {
        class Widget : public myos::drivers::KeyboardEventHandler
        {
            protected:
                Widget* parent;
                myos::common::int32_t x;
                myos::common::int32_t y;
                myos::common::int32_t w;
                myos::common::int32_t h;

                myos::common::uint8_t r;
                myos::common::uint8_t g;
                myos::common::uint8_t b;
                bool Focusable;

            public:
                Widget(
                    Widget* parent,
                    myos::common::int32_t x,
                    myos::common::int32_t y,
                    myos::common::int32_t w,
                    myos::common::int32_t h,
                    myos::common::uint8_t r,
                    myos::common::uint8_t g,
                    myos::common::uint8_t b);
                ~Widget();



                virtual void GetFocus(Widget* widget);
                virtual void ModelToScreen(myos::common::int32_t &x, myos::common::int32_t &y);
                virtual bool ContainsCoordinate(myos::common::int32_t x, myos::common::int32_t y);

                virtual void Draw(myos::common::GraphicsContext* gc);
                virtual void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                virtual void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                virtual void OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY);                 


        };

        class CompositeWidget : public Widget
        {
            private:
                Widget* children[100];
                int numChildren;
                Widget* focusedChild;

            public:
                CompositeWidget(
                    Widget* parent,
                    myos::common::int32_t x,
                    myos::common::int32_t y,
                    myos::common::int32_t w,
                    myos::common::int32_t h,
                    myos::common::uint8_t r,
                    myos::common::uint8_t g,
                    myos::common::uint8_t b);
                ~CompositeWidget();


                virtual void GetFocus(Widget* widget);
                virtual bool AddChild(Widget* child);

                virtual void Draw(myos::common::GraphicsContext* gc);
                virtual void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                virtual void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);                
                virtual void OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY);

 
                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);

        };
    }
}


#endif