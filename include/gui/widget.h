#ifndef __MYOS__GUI__WIDGET__H
#define __MYOS__GUI__WIDGET__H

#include <common/types.h>
#include <common/graphicsContext.h>

namespace myos 
{
    namespace gui
    {
        class Widget
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

                virtual void Draw(myos::common::GraphicsContext* gc);
                virtual void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y);                
                virtual void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y);                
                virtual void OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY);                 

 
                virtual void OnKeyDown(char* str);
                virtual void OnKeyUp(char* str);

                virtual bool ContainsCoordinate(myos::common::int32_t x, myos::common::int32_t y);

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

                virtual void Draw(myos::common::GraphicsContext* gc);
                virtual void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y);                
                virtual void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y);                
                virtual void OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY);

 
                virtual void OnKeyDown(char* str);
                virtual void OnKeyUp(char* str);

        };
    }
}


#endif